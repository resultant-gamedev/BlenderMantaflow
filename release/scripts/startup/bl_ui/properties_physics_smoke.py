# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8 compliant>
import bpy
import os
from copy import deepcopy
from bpy.types import Panel

from bl_ui.properties_physics_common import (
        point_cache_ui,
        effector_weights_ui,
        )


class PhysicButtonsPanel:
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "physics"

    @classmethod
    def poll(cls, context):
        ob = context.object
        rd = context.scene.render
        return (ob and ob.type == 'MESH') and (rd.engine in cls.COMPAT_ENGINES) and (context.smoke)


class PHYSICS_PT_smoke(PhysicButtonsPanel, Panel):
    bl_label = "Fluid"
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    def draw(self, context):
        layout = self.layout

        if not bpy.app.build_options.mod_smoke:
            layout.label("Built without Smoke modifier")
            return

        md = context.smoke
        ob = context.object
        scene = context.scene

        layout.prop(md, "smoke_type", expand=True)

        if md.smoke_type == 'DOMAIN':
            domain = md.domain_settings
            flow = md.flow_settings

            layout.prop(domain, "smoke_domain_type", expand=False)
            layout.prop(domain, "collision_extents", expand=False)

            split = layout.split()

            split.enabled = not domain.point_cache.is_baked

            col = split.column()
            col.label(text="Time:")
            col.prop(domain, "time_scale", text="Scale")
            col.prop(domain, "use_adaptive_stepping", text="Adaptive stepping")
            
            col = split.column()
            if scene.use_gravity:
                col.label(text="Use Scene Gravity", icon='SCENE_DATA')
                sub = col.column()
                sub.enabled = False
                sub.prop(domain, "gravity", text="")
            else:
                col.label(text="Gravity:")
                col.prop(domain, "gravity", text="")

            if domain.smoke_domain_type in {'GAS'}:
                split = layout.split()
                split.enabled = not domain.point_cache.is_baked

                col = split.column()
                col.label(text="Smoke:")
                col.prop(domain, "alpha")
                col.prop(domain, "beta", text="Temp. Diff.")
                col.prop(domain, "vorticity")
                col.prop(domain, "use_dissolve_smoke", text="Dissolve")
                sub = col.column()
                sub.active = domain.use_dissolve_smoke
                sub.prop(domain, "dissolve_speed", text="Time")
                sub.prop(domain, "use_dissolve_smoke_log", text="Slow")

                col = split.column()
                col.label(text="Fire:")
                col.prop(domain, "burning_rate")
                col.prop(domain, "flame_smoke")
                col.prop(domain, "flame_vorticity")
                col.prop(domain, "flame_ignition")
                col.prop(domain, "flame_max_temp")
                col.prop(domain, "flame_smoke_color")

            if domain.smoke_domain_type in {'LIQUID'}:
                split = layout.split()
                split.enabled = not domain.point_cache.is_baked

                col = split.column()
                col.label(text="Liquid:")
                col.prop(domain, "particle_randomness")
                col.prop(domain, "particle_radius")

                col = split.column()
                col.label()
                col.prop(domain, "particle_number")

        elif md.smoke_type == 'FLOW':
            flow = md.flow_settings

            layout.prop(flow, "smoke_flow_type", expand=False)

            split = layout.split()
            col = split.column()
                
            col.label(text="Sampling:")
            col.prop(flow, "subframes")

            col = split.column()
            
            col.label(text="Flow behavior:")
            col.prop(flow, "smoke_flow_behavior", expand=False, text="")

            if not flow.smoke_flow_behavior == 'OUTFLOW':

                split = layout.split()
                col = split.column()

                if flow.smoke_flow_type in {'SMOKE', 'BOTH', 'FIRE'}:
                    col.label(text="Initial Values:")
                if flow.smoke_flow_type in {'SMOKE', 'BOTH'}:
                    col.prop(flow, "density")
                    col.prop(flow, "temperature")
                if flow.smoke_flow_type in {'FIRE', 'BOTH'}:
                    col.prop(flow, "fuel_amount")

                col = split.column()

                if flow.smoke_flow_behavior in {'INFLOW', 'GEOMETRY'}:
                    col.prop(flow, "use_inflow")
                if flow.smoke_flow_type in {'SMOKE', 'BOTH', 'FIRE'}:
                    col.prop(flow, "use_absolute")
                if flow.smoke_flow_type in {'SMOKE', 'BOTH'}:
                    col.prop(flow, "smoke_color")

        elif md.smoke_type == 'COLLISION':
            coll = md.coll_settings

            split = layout.split()

            col = split.column()
            col.prop(coll, "collision_type")

class PHYSICS_PT_smoke_flow_advanced(PhysicButtonsPanel, Panel):
    bl_label = "Fluid Source"
    bl_options = {'DEFAULT_CLOSED'}
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        return md and (md.smoke_type == 'FLOW')

    def draw(self, context):
        layout = self.layout
        ob = context.object
        flow = context.smoke.flow_settings
        
        split = layout.split()
        
        col = split.column()
        col.label(text="Flow Source:")
        col.prop(flow, "smoke_flow_source", expand=False, text="")
        if flow.smoke_flow_source == 'MESH':
            col.prop(flow, "surface_distance")
            col.prop(flow, "volume_density")

        sub = col.column(align=True)
        sub.prop(flow, "use_initial_velocity")

        sub = sub.column()
        sub.active = flow.use_initial_velocity
        sub.prop(flow, "velocity_factor")
        if flow.smoke_flow_source == 'MESH':
            sub.prop(flow, "velocity_normal")
            #sub.prop(flow, "velocity_random")

        col = split.column()
        if flow.smoke_flow_source == 'PARTICLES':
            col.label(text="Particle System:")
            col.prop_search(flow, "particle_system", ob, "particle_systems", text="")
            col.prop(flow, "use_particle_size", text="Set Size")
            sub = col.column()
            sub.active = flow.use_particle_size
            sub.prop(flow, "particle_size")
        else:
            col.label(text="Vertex Group:")
            col.prop_search(flow, "density_vertex_group", ob, "vertex_groups", text="")
            
            col.prop(flow, "use_texture")
            sub = col.column()
            sub.active = flow.use_texture
            sub.prop(flow, "noise_texture", text="")
            sub.label(text="Mapping:")
            sub.prop(flow, "texture_map_type", expand=False, text="")
            if flow.texture_map_type == 'UV':
                sub.prop_search(flow, "uv_layer", ob.data, "uv_textures", text="")
            if flow.texture_map_type == 'AUTO':
                sub.prop(flow, "texture_size")
            sub.prop(flow, "texture_offset")

class PHYSICS_PT_smoke_adaptive_domain(PhysicButtonsPanel, Panel):
    bl_label = "Smoke Adaptive Domain"
    bl_options = {'DEFAULT_CLOSED'}
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        # TODO (sebbas): Adaptive domain currently not supported by mantaflow. Disabling for now
        return False
        # return md and (md.smoke_type == 'DOMAIN')

    def draw_header(self, context):
        md = context.smoke.domain_settings

        self.layout.prop(md, "use_adaptive_domain", text="")
        
    def draw(self, context):
        layout = self.layout

        domain = context.smoke.domain_settings
        layout.active = domain.use_adaptive_domain
        
        split = layout.split()
        split.enabled = (not domain.point_cache.is_baked)

        col = split.column(align=True)
        col.label(text="Resolution:")
        col.prop(domain, "additional_res")
        col.prop(domain, "adapt_margin")

        col = split.column(align=True)
        col.label(text="Advanced:")
        col.prop(domain, "adapt_threshold")


class PHYSICS_PT_smoke_highres(PhysicButtonsPanel, Panel):
    bl_label = "Fluid Quality"
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        rd = context.scene.render
        return md and (md.smoke_type == 'DOMAIN') and (rd.engine in cls.COMPAT_ENGINES)

    def draw(self, context):
        layout = self.layout
        domain = context.smoke.domain_settings

        split = layout.split()

        col = split.column()
        col.enabled = not domain.point_cache.is_baked
        col.label(text="Resolution:")
        col.prop(domain, "resolution_max", text="Divisions")
        # TODO (sebbas): Disabling render display switch for now. Needs some more consideration
        #col.label(text="Render Display:")
        #col.prop(domain, "render_display_mode", text="")

        col = split.column()
        #col.label()
        #col.label()
        col.label(text="Viewport Display:")
        col.prop(domain, "viewport_display_mode", text="")

        split = layout.split()
        split.enabled = not domain.point_cache.is_baked

        if domain.smoke_domain_type == 'LIQUID':
            col = split.column()
            col.prop(domain, "use_high_resolution", text="High resolution")
            sub = col.column()
            sub.active = domain.use_high_resolution
            sub.prop(domain, "amplify", text="Divisions")
            
            sub = split.column()
            sub.active = domain.use_high_resolution
            sub.label(text="Flow Sampling:")
            sub.row().prop(domain, "highres_sampling", text="")
            
        if domain.smoke_domain_type == 'GAS':
            col = split.column()
            col.prop(domain, "use_high_resolution", text="High resolution")
            sub = col.column()
            sub.active = domain.use_high_resolution
            sub.prop(domain, "amplify", text="Divisions")
            sub.label(text="Flow Sampling:")
            sub.row().prop(domain, "highres_sampling", text="")
        
            sub = split.column()
            sub.active = domain.use_high_resolution
            # TODO (sebbas): Mantaflow only supports wavelet noise. Do we really need fft noise? Maybe get rid of noise type ...
            #sub.label(text="Noise Method:")
            #sub.row().prop(domain, "noise_type", text="")
            sub.label(text="Noise:")
            sub.prop(domain, "strength")
            sub.prop(domain, "noise_pos_scale")
            sub.prop(domain, "noise_time_anim")

class PHYSICS_PT_smoke_groups(PhysicButtonsPanel, Panel):
    bl_label = "Fluid Groups"
    bl_options = {'DEFAULT_CLOSED'}
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        rd = context.scene.render
        return md and (md.smoke_type == 'DOMAIN') and (rd.engine in cls.COMPAT_ENGINES)

    def draw(self, context):
        layout = self.layout
        domain = context.smoke.domain_settings

        split = layout.split()

        col = split.column()
        col.label(text="Flow Group:")
        col.prop(domain, "fluid_group", text="")

        #col.label(text="Effector Group:")
        #col.prop(domain, "effector_group", text="")

        col = split.column()
        col.label(text="Collision Group:")
        col.prop(domain, "collision_group", text="")


class PHYSICS_PT_smoke_cache(PhysicButtonsPanel, Panel):
    bl_label = "Fluid Cache"
    bl_options = {'DEFAULT_CLOSED'}
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        rd = context.scene.render
        return md and (md.smoke_type == 'DOMAIN') and (rd.engine in cls.COMPAT_ENGINES)

    def draw(self, context):
        layout = self.layout

        domain = context.smoke.domain_settings
        cache_file_format = domain.cache_file_format

        layout.prop(domain, "cache_file_format")

        if cache_file_format == 'POINTCACHE':
            layout.label(text="Compression:")
            layout.prop(domain, "point_cache_compress_type", expand=True)
        elif cache_file_format == 'OPENVDB':
            if not bpy.app.build_options.openvdb:
                layout.label("Built without OpenVDB support")
                return

            layout.label(text="Compression:")
            layout.prop(domain, "openvdb_cache_compress_type", expand=True)
            row = layout.row()
            row.label("Data Depth:")
            row.prop(domain, "data_depth", expand=True, text="Data Depth")
        elif cache_file_format == 'OBJECT':
            layout.label(text="Compression:")
            layout.prop(domain, "liquid_cache_compress_type", expand=True)

        cache = domain.point_cache
        point_cache_ui(self, context, cache, (cache.is_baked is False), 'SMOKE')


class PHYSICS_PT_smoke_field_weights(PhysicButtonsPanel, Panel):
    bl_label = "Fluid Field Weights"
    bl_options = {'DEFAULT_CLOSED'}
    COMPAT_ENGINES = {'BLENDER_RENDER'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        rd = context.scene.render
        return md and (md.smoke_type == 'DOMAIN') and (rd.engine in cls.COMPAT_ENGINES)

    def draw(self, context):
        domain = context.smoke.domain_settings
        effector_weights_ui(self, context, domain.effector_weights, 'SMOKE')

class OBJECT_OT_RunMantaButton(bpy.types.Operator):
    bl_idname = "manta_export_scene.button"
    bl_label = "Create Python Script"
    
    def execute(self, context):
        bpy.ops.manta.make_file()
        return{'FINISHED'}

class PHYSICS_PT_smoke_manta_settings(PhysicButtonsPanel, Panel):
    bl_label = "Fluid Export"
    bl_options = {'DEFAULT_CLOSED'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
        return md and (md.smoke_type == 'DOMAIN')
        
    def draw(self, context):
        layout = self.layout
        
        domain = context.smoke.domain_settings
        split = layout.split()
        split.operator("manta_export_scene.button", text="Export Mantaflow Script")
        split = layout.split()
        split.prop(domain, "manta_filepath")
        split = layout.split()

class PHYSICS_PT_smoke_display_settings(PhysicButtonsPanel, Panel):
    bl_label = "Smoke Display Settings"
    bl_options = {'DEFAULT_CLOSED'}

    @classmethod
    def poll(cls, context):
        md = context.smoke
	
        rd = context.scene.render
        return md and (md.smoke_type == 'DOMAIN') and (not rd.use_game_engine)

    def draw(self, context):
        domain = context.smoke.domain_settings
        layout = self.layout

        layout.prop(domain, "display_thickness")

        layout.separator()
        layout.label(text="Slicing:")
        layout.prop(domain, "slice_method")

        slice_method = domain.slice_method
        axis_slice_method = domain.axis_slice_method

        do_axis_slicing = (slice_method == 'AXIS_ALIGNED')
        do_full_slicing = (axis_slice_method == 'FULL')

        row = layout.row();
        row.enabled = do_axis_slicing
        row.prop(domain, "axis_slice_method")

        col = layout.column();
        col.enabled = not do_full_slicing and do_axis_slicing
        col.prop(domain, "slice_axis")
        col.prop(domain, "slice_depth")

        row = layout.row();
        row.enabled = do_full_slicing or not do_axis_slicing
        row.prop(domain, "slice_per_voxel")

        layout.separator()
        layout.label(text="Debug:")
        layout.prop(domain, "draw_velocity")
        col = layout.column();
        col.enabled = domain.draw_velocity
        col.prop(domain, "vector_draw_type")
        col.prop(domain, "vector_scale")

        layout.separator()
        layout.label(text="Color Mapping:")
        layout.prop(domain, "use_color_ramp")
        col = layout.column();
        col.enabled = domain.use_color_ramp
        col.prop(domain, "coba_field")
        col.template_color_ramp(domain, "color_ramp", expand=True)

if __name__ == "__main__":  # only for live edit.
    bpy.utils.register_module(__name__)
