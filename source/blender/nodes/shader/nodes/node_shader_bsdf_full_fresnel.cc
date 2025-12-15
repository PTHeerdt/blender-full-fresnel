//*******************************************************************************************************************************
//START CODE PETER TER HEERDT UANTWERPEN*****************************************************************************************
//*******************************************************************************************************************************

/* SPDX-FileCopyrightText: 2024 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "node_shader_util.hh"

#include "UI_interface.hh"
#include "UI_resources.hh"

namespace blender::nodes::node_shader_bsdf_full_fresnel_cc {

static void node_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>("Base Color")
      .default_value({0.617f, 0.577f, 0.540f, 1.0f})
      .description("Color of the material");
  b.add_input<decl::Vector>("n Object")
      .default_value({1.5f, 1.5f, 1.5f})
      .min(0.0f)
      .max(100.0f)
      .description("Real part of the object's refractive index");
  b.add_input<decl::Vector>("k Object")
      .default_value({0.f, 0.f, 0.f})
      .min(0.0f)
      .max(100.0f)
      .description("Imaginary part of the object's refractive index");
  b.add_input<decl::Vector>("n Medium")
      .default_value({1.f, 1.f, 1.f})
      .min(0.0f)
      .max(100.0f)
      .description("Real part of the medium's refractive index");
  b.add_input<decl::Vector>("k Medium")
      .default_value({0.f, 0.f, 0.f})
      .min(0.0f)
      .max(100.0f)
      .description("Imaginary part of the medium's refractive index");
  b.add_input<decl::Float>("Roughness")
      .default_value(0.f)
      .min(0.f)
      .max(1.f)
      .subtype(PROP_FACTOR)
      .description(
          "Microfacet roughness of the surface (0.0 is a perfect mirror reflection, 1.0 is "
          "completely rough)");
  ;
  b.add_input<decl::Float>("Anisotropy")
      .default_value(0.f)
      .min(0.f)
      .max(1.f)
      .subtype(PROP_FACTOR)
      .description(
          "Amount of anisotropy for reflection. Higher values give elongated highlights along the "
          "tangent direction");
  b.add_input<decl::Float>("Rotation")
      .default_value(0.f)
      .min(0.f)
      .max(1.f)
      .subtype(PROP_FACTOR)
      .description("Rotates the direction of anisotropy, with 1.0 going full circle");
  b.add_input<decl::Vector>("Normal").hide_value();
  b.add_input<decl::Vector>("Tangent").hide_value();
  b.add_input<decl::Float>("Weight").available(false);
  b.add_output<decl::Shader>("BSDF");

  b.add_input<decl::Float>("Temperature")
      .default_value(0.0f)
      .min(0.0f)
      .max(7000.0f)
      .subtype(PROP_TEMPERATURE)
      .description("Blackbody temperature in Kelvin. 0 disables thermal emission");
}

static void node_shader_buts_full_fresnel(uiLayout *layout, bContext * /*C*/, PointerRNA *ptr)
{
  uiItemR(layout, ptr, "distribution", UI_ITEM_R_SPLIT_EMPTY_NAME, "", ICON_NONE);
}

static void node_shader_init_full_fresnel(bNodeTree * /*ntree*/, bNode *node)
{
  node->custom1 = SHD_GLOSSY_MULTI_GGX;
}

static int node_shader_gpu_bsdf_full_fresnel(GPUMaterial *mat,
                                         bNode *node,
                                         bNodeExecData * /*execdata*/,
                                         GPUNodeStack *in,
                                         GPUNodeStack *out)
{
  //In node_declare(...) the "Normal" input parameter is at the 9th place.
  //so [8] when indexing starts at 0.
  if (!in[8].link) {
    GPU_link(mat, "world_normals_get", &in[8].link);
  }

  GPU_material_flag_set(mat, GPU_MATFLAG_GLOSSY | GPU_MATFLAG_REFRACT);

  float use_multi_scatter = (node->custom1 == SHD_GLOSSY_MULTI_GGX) ? 1.0f : 0.0f;

  return GPU_stack_link(mat,
                        node,
                        "node_bsdf_full_fresnel",
                        in,
                        out,
                        GPU_constant(&use_multi_scatter));
}

NODE_SHADER_MATERIALX_BEGIN
#ifdef WITH_MATERIALX
{
  if (to_type_ != NodeItem::Type::BSDF) {
    return empty();
  }

  NodeItem color = get_input_value("Base Color", NodeItem::Type::Color3);
  NodeItem ior_object = get_input_value("n Object", NodeItem::Type::Color3);
  NodeItem k_object = get_input_value("k Object", NodeItem::Type::Color3);
  NodeItem ior_medium = get_input_value("n Medium", NodeItem::Type::Color3);
  NodeItem k_medium = get_input_value("k Medium", NodeItem::Type::Color3);
  NodeItem roughness = get_input_value("Roughness", NodeItem::Type::Vector2);
  NodeItem anisotropy = get_input_value("Anisotropy", NodeItem::Type::Color3);
  NodeItem rotation = get_input_value("Rotation", NodeItem::Type::Color3);
  NodeItem normal = get_input_link("Normal", NodeItem::Type::Vector3);
  NodeItem tangent = get_input_link("Tangent", NodeItem::Type::Vector3);
  NodeItem temperature = get_input_link("Temperature", NodeItem::Type::Float);

   return create_node("full_fresnel_bsdf",
                      NodeItem::Type::BSDF,
                      {{"Base Color", color},
                       {"n Object", ior_object},
                       {"k Object", k_object},
                       {"n Medium", ior_medium},
                       {"k Medium", k_medium},
                       {"Roughness", roughness},
                       {"Anisotropy", anisotropy},
                       {"Rotation", rotation},
                       {"Normal", normal},
                       {"Tangent", tangent},
                       {"Temperature", temperature}});
}
#endif
NODE_SHADER_MATERIALX_END

}  // namespace blender::nodes::node_shader_bsdf_full_fresnel_cc

/* node type definition */
void register_node_type_sh_bsdf_full_fresnel()
{
  namespace file_ns = blender::nodes::node_shader_bsdf_full_fresnel_cc;

  static blender::bke::bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_BSDF_FULL_FRESNEL, "Full Fresnel BSDF", NODE_CLASS_SHADER);
  ntype.declare = file_ns::node_declare;
  ntype.add_ui_poll = object_shader_nodes_poll;
  ntype.draw_buttons = file_ns::node_shader_buts_full_fresnel;
  blender::bke::node_type_size_preset(&ntype, blender::bke::eNodeSizePreset::Large);
  ntype.initfunc = file_ns::node_shader_init_full_fresnel;
  ntype.gpu_fn = file_ns::node_shader_gpu_bsdf_full_fresnel;
  ntype.materialx_fn = file_ns::node_shader_materialx;

  blender::bke::node_register_type(&ntype);
}

//*******************************************************************************************************************************
//END CODE PETER TER HEERDT UANTWERPEN*******************************************************************************************
//*******************************************************************************************************************************
