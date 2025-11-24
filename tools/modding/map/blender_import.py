import bpy
import bmesh
import struct
import math
import os
import traceback

# =============================================================================
# CONFIGURATION
# =============================================================================
MAP_NAME = "farm-spring" 
# Replace with absolute path to map assets directory
BASE_PATH = rf"..\..\..\assets\maps"
MAP_FILE_PATH = rf"{BASE_PATH}\{MAP_NAME}\{MAP_NAME}.bin" 
TEXTURES_BASE_PATH = rf"{BASE_PATH}\{MAP_NAME}\tiles"
CORE_OBJ_TEXTURES_PATH = rf"{BASE_PATH}\{MAP_NAME}\core_map_objects"

SCALE_FACTOR = 0.1
# Map tiles are rendered with the OPA_SURF (Opaque Surface) flag (transparency bit is ignored).
USE_TEXTURE_ALPHA = False
FORCE_WHITE_VERTEX_COLORS = True
APPLY_TEXTURES = True
RENDER_CORE_MAP_OBJECTS = True

# =============================================================================
# HELPERS
# =============================================================================
def check_paths():
    print("--- CHECKING PATHS ---")
    if not os.path.exists(MAP_FILE_PATH):
        print(f"Map file missing: {MAP_FILE_PATH}")
        return False
    if not os.path.exists(TEXTURES_BASE_PATH):
        print(f"Texture folder missing: {TEXTURES_BASE_PATH}")
        return False
    
    files = os.listdir(TEXTURES_BASE_PATH)
    pngs = [f for f in files if f.endswith('.png')]

    if len(pngs) <= 0:
        print("No PNG files found! Model will not be textured.")

    return True

# =============================================================================
# NODE GROUPS
# =============================================================================
def create_atlas_wrapper_group():
    group_name = "HM64_Atlas_Wrap"
    if group_name in bpy.data.node_groups:
        return bpy.data.node_groups[group_name]
    
    group = bpy.data.node_groups.new(group_name, 'ShaderNodeTree')
    group.interface.new_socket("UV", in_out='INPUT', socket_type='NodeSocketVector')
    group.interface.new_socket("Min U", in_out='INPUT', socket_type='NodeSocketFloat')
    group.interface.new_socket("Max U", in_out='INPUT', socket_type='NodeSocketFloat')
    group.interface.new_socket("Min V", in_out='INPUT', socket_type='NodeSocketFloat')
    group.interface.new_socket("Max V", in_out='INPUT', socket_type='NodeSocketFloat')
    group.interface.new_socket("Vector", in_out='OUTPUT', socket_type='NodeSocketVector')
    
    nodes = group.nodes
    links = group.links
    
    input_node = nodes.new('NodeGroupInput')
    input_node.location = (-400, 0)
    output_node = nodes.new('NodeGroupOutput')
    output_node.location = (400, 0)
    
    # Set Defaults
    input_node.outputs['Max U'].default_value = 1.0
    input_node.outputs['Max V'].default_value = 1.0

    sep = nodes.new('ShaderNodeSeparateXYZ')
    links.new(input_node.outputs['UV'], sep.inputs['Vector'])
    
    # U Logic
    sub_u = nodes.new('ShaderNodeMath')
    sub_u.operation = 'SUBTRACT'
    links.new(input_node.outputs['Max U'], sub_u.inputs[0])
    links.new(input_node.outputs['Min U'], sub_u.inputs[1])
    
    mod_u = nodes.new('ShaderNodeMath')
    mod_u.operation = 'FLOORED_MODULO'
    links.new(sep.outputs['X'], mod_u.inputs[0])
    links.new(sub_u.outputs[0], mod_u.inputs[1])
    
    add_u = nodes.new('ShaderNodeMath')
    add_u.operation = 'ADD'
    links.new(mod_u.outputs[0], add_u.inputs[0])
    links.new(input_node.outputs['Min U'], add_u.inputs[1])
    
    # V Logic
    sub_v = nodes.new('ShaderNodeMath')
    sub_v.operation = 'SUBTRACT'
    links.new(input_node.outputs['Max V'], sub_v.inputs[0])
    links.new(input_node.outputs['Min V'], sub_v.inputs[1])
    
    mod_v = nodes.new('ShaderNodeMath')
    mod_v.operation = 'FLOORED_MODULO'
    links.new(sep.outputs['Y'], mod_v.inputs[0])
    links.new(sub_v.outputs[0], mod_v.inputs[1])
    
    add_v = nodes.new('ShaderNodeMath')
    add_v.operation = 'ADD'
    links.new(mod_v.outputs[0], add_v.inputs[0])
    links.new(input_node.outputs['Min V'], add_v.inputs[1])
    
    comb = nodes.new('ShaderNodeCombineXYZ')
    links.new(add_u.outputs[0], comb.inputs['X'])
    links.new(add_v.outputs[0], comb.inputs['Y'])
    
    links.new(comb.outputs['Vector'], output_node.inputs['Vector'])
    
    return group

# =============================================================================
# MATERIAL LOADER
# =============================================================================
def get_material(texture_index, base_path, solid_color=None):
    mat_name = f"Material_Tile_{texture_index}"
    if solid_color: mat_name += "_Solid"
    
    # Check cache
    if mat_name in bpy.data.materials:
        return bpy.data.materials[mat_name]
    
    mat = bpy.data.materials.new(name=mat_name)
    mat.use_nodes = True
    nodes = mat.node_tree.nodes
    links = mat.node_tree.links
    for n in nodes: nodes.remove(n)
    
    output = nodes.new('ShaderNodeOutputMaterial')
    bsdf = nodes.new('ShaderNodeBsdfPrincipled')
    
    tex_filename = f"tile-{texture_index}.png"
    tex_path = os.path.join(base_path, tex_filename)
    
    has_texture = False
    
    # Only load texture if NOT solid color override and ID is valid
    if solid_color is None and texture_index < 80 and os.path.exists(tex_path):
        try:
            img = bpy.data.images.load(tex_path)
            img.colorspace_settings.name = 'sRGB'
            
            tex_node = nodes.new('ShaderNodeTexImage')
            tex_node.image = img
            tex_node.interpolation = 'Closest'
            tex_node.extension = 'CLIP'
            
            wrapper_group = create_atlas_wrapper_group()
            wrapper = nodes.new('ShaderNodeGroup')
            wrapper.node_tree = wrapper_group
            
            wrapper.inputs['Min U'].default_value = 0.0
            wrapper.inputs['Max U'].default_value = 1.0
            wrapper.inputs['Min V'].default_value = 0.0
            wrapper.inputs['Max V'].default_value = 1.0
            
            uv_map = nodes.new('ShaderNodeUVMap')
            
            links.new(uv_map.outputs['UV'], wrapper.inputs['UV'])
            links.new(wrapper.outputs['Vector'], tex_node.inputs['Vector'])
            links.new(tex_node.outputs['Color'], bsdf.inputs['Base Color'])
            
            if USE_TEXTURE_ALPHA:
                 links.new(tex_node.outputs['Alpha'], bsdf.inputs['Alpha'])
                 
            has_texture = True

        except Exception as e:
            print(f"Failed to load texture {tex_filename}: {e}")

    if not has_texture:
        if solid_color:
            bsdf.inputs['Base Color'].default_value = solid_color
        else:
            # ERROR PINK
            bsdf.inputs['Base Color'].default_value = (1.0, 0.0, 1.0, 1.0)

    links.new(bsdf.outputs['BSDF'], output.inputs['Surface'])
    return mat

def get_sprite_material(sprite_index, base_path):
    mat_name = f"Mat_Sprite_{sprite_index}"
    
    if mat_name in bpy.data.materials:
        return bpy.data.materials[mat_name]
    
    mat = bpy.data.materials.new(name=mat_name)
    mat.use_nodes = True
    
    # --- TRANSPARENCY SETTINGS ---
    # 'CLIP' makes the alpha channel act like a hard cutout (0 or 1)
    # This matches the N64 alpha testing behavior
    mat.blend_method = 'CLIP' 
    mat.alpha_threshold = 0.5
    mat.show_transparent_back = True 
    
    nodes = mat.node_tree.nodes
    links = mat.node_tree.links
    nodes.clear()
    
    output = nodes.new('ShaderNodeOutputMaterial')
    bsdf = nodes.new('ShaderNodeBsdfPrincipled')
    
    # Connect BSDF to Output
    links.new(bsdf.outputs['BSDF'], output.inputs['Surface'])
    
    # Texture Setup
    tex_filename = f"map_object-{sprite_index}.png"
    tex_path = os.path.join(base_path, tex_filename)
    
    if os.path.exists(tex_path):
        try:
            # Ensure fresh load
            if tex_filename in bpy.data.images:
                img = bpy.data.images[tex_filename]
                img.reload()
            else:
                img = bpy.data.images.load(tex_path)
            
            img.colorspace_settings.name = 'sRGB'
            img.alpha_mode = 'STRAIGHT'
            
            tex_node = nodes.new('ShaderNodeTexImage')
            tex_node.image = img
            tex_node.interpolation = 'Closest'
            
            # Reconnect alpha
            links.new(tex_node.outputs['Color'], bsdf.inputs['Base Color'])
            
            if img.depth in (32, 128): 
                links.new(tex_node.outputs['Alpha'], bsdf.inputs['Alpha'])
            else:
                bsdf.inputs['Alpha'].default_value = 1.0
                
        except Exception as e:
            print(f"Texture Error: {e}")
            bsdf.inputs['Base Color'].default_value = (1.0, 0.0, 1.0, 1.0) # Pink Error
    else:
        bsdf.inputs['Base Color'].default_value = (1.0, 0.0, 1.0, 1.0) # Pink Error

    return mat

# =============================================================================
# CORE MAP OBJECTS
# =============================================================================
def render_core_map_objects(reader, offset_objects, collection, map_dims):
    print("--- IMPORTING CORE MAP OBJECTS ---")
    if offset_objects == 0: return

    map_w, map_h, size_x, size_z = map_dims
    
    world_center_x = (map_w * size_x) / 2.0
    world_center_z = (map_h * size_z) / 2.0
    tile_offset_x = size_x / 2.0
    tile_offset_z = size_z / 2.0

    reader.seek(offset_objects + 4)
    total_groups = reader.read_u8()
    current_pos = offset_objects + 5 
    
    print(f"Processing {total_groups} object groups...")
    cam_obj = bpy.data.objects.get("HM64_Camera")
    
    for i in range(total_groups):
        reader.seek(current_pos)
        
        sprite_index = reader.read_u8()
        repeat_count = reader.read_u8()
        
        mat = get_sprite_material(sprite_index, CORE_OBJ_TEXTURES_PATH)
        tex_w, tex_h = 32.0, 32.0

        if mat.node_tree:
            nodes = mat.node_tree.nodes
            t_node = next((n for n in nodes if n.type == 'TEX_IMAGE'), None)
            if t_node and t_node.image:
                tex_w = float(t_node.image.size[0])
                tex_h = float(t_node.image.size[1])

        mesh_name = f"Sprite_Mesh_{sprite_index}"
        mesh = bpy.data.meshes.get(mesh_name)

        if not mesh:
            mesh = bpy.data.meshes.new(mesh_name)
            bm = bmesh.new()
            
            # Center the mesh (-H/2 to H/2)
            final_w = (tex_w * SCALE_FACTOR) / 2
            final_h = (tex_h * SCALE_FACTOR) / 2
            
            v1 = bm.verts.new((-final_w, -final_h, 0)) 
            v2 = bm.verts.new((final_w, -final_h, 0))  
            v3 = bm.verts.new((final_w, final_h, 0))   
            v4 = bm.verts.new((-final_w, final_h, 0))  
            
            face = bm.faces.new((v1, v2, v3, v4))
            
            uv_layer = bm.loops.layers.uv.new()
            face.loops[0][uv_layer].uv = (0, 0)
            face.loops[1][uv_layer].uv = (1, 0)
            face.loops[2][uv_layer].uv = (1, 1)
            face.loops[3][uv_layer].uv = (0, 1)
            
            bm.to_mesh(mesh)
            bm.free()
            mesh.materials.append(mat)

        for j in range(repeat_count):
            flags = reader.read_u8()
            raw_x = reader.read_s16_le()
            raw_y = reader.read_s16_le()
            raw_z = reader.read_s16_le()
            
            # Coordinates
            n64_x_absolute = raw_x + world_center_x - tile_offset_x
            n64_z_absolute = raw_z + world_center_z - tile_offset_z
            
            wx = n64_x_absolute * SCALE_FACTOR
            wy = raw_y * SCALE_FACTOR
            wz = -(n64_z_absolute * SCALE_FACTOR)
            
            # Scale
            scale_bits = flags & 0x0C
            scale_mult = 1.0
            if scale_bits == 0x04: scale_mult = 2.0
            elif scale_bits == 0x08: scale_mult = 4.0
            elif scale_bits == 0x0C: scale_mult = 8.0
            
            obj = bpy.data.objects.new(f"Obj_{sprite_index}_{i}_{j}", mesh)
            collection.objects.link(obj)
            obj.location = (wx, wz, wy) 
            obj.scale = (scale_mult, scale_mult, scale_mult)
            
            # --- ROTATION ---
            # If flag 0x80 is set, use fixed rotation (windows, walls)
            # If flag 0x80 is NOT set, use billboard (trees, signs)
            
            if flags & 0x80:
                rot_bits = flags & 0x70
                angle_deg = 0
                if rot_bits == 0x70: angle_deg = 45
                elif rot_bits == 0x10: angle_deg = 315
                elif rot_bits == 0x20: angle_deg = 270
                elif rot_bits == 0x30: angle_deg = 225
                elif rot_bits == 0x40: angle_deg = 180
                elif rot_bits == 0x50: angle_deg = 135
                elif rot_bits == 0x60: angle_deg = 90
                
                obj.rotation_euler = (math.radians(90), 0, math.radians(angle_deg))
                
            else:
                # BILLBOARD MODE
                if cam_obj:
                    const = obj.constraints.new(type='COPY_ROTATION')
                    const.target = cam_obj
                    const.use_x = True; const.use_y = True; const.use_z = True

        current_pos += 2 + (7 * repeat_count)

# =============================================================================
# CAMERA SETUP
# =============================================================================
def setup_camera(map_dims):
    """
    Sets up an Isometric Orthographic camera to match graphic.c logic.
    map_dims = (map_width, map_height, tile_size_x, tile_size_z)
    """
    print("--- SETTING UP CAMERA ---")
    
    map_width, map_height, tile_size_x, tile_size_z = map_dims
    
    # Calculate the physical center of the map in Blender units
    # Map game Z to Blender Y
    center_x = (map_width * tile_size_x * SCALE_FACTOR) / 2
    center_y = -((map_height * tile_size_z * SCALE_FACTOR) / 2) 
    
    # 1. Create camera data
    cam_data = bpy.data.cameras.new("HM64_Camera")
    cam_data.type = 'ORTHO'
    
    # 320 = N64 screen width
    cam_data.ortho_scale = 320.0 * SCALE_FACTOR 

    # 2. Create camera object
    cam_obj = bpy.data.objects.new("HM64_Camera", cam_data)
    bpy.context.collection.objects.link(cam_obj)
    
    # 3. Position and rotate
    # Move camera away from the map center to prevent clipping
    cam_obj.location = (center_x + 20, center_y - 20, 30)
    
    # Apply rotation angles found in graphic.c (converted to radians)
    # X = 45 degrees (tilt)
    # Z = 45 degrees (isometric spin)
    cam_obj.rotation_euler = (math.radians(45.0), 0, math.radians(45.0)) #
    
    # Set as active camera
    bpy.context.scene.camera = cam_obj
    
    return cam_obj

# =============================================================================
# READER
# =============================================================================
class BinaryReader:
    def __init__(self, filepath):
        self.file = open(filepath, 'rb')
        self.filesize = os.path.getsize(filepath)
    def close(self): self.file.close()
    def seek(self, offset): self.file.seek(offset)
    def tell(self): return self.file.tell()
    def read_u8(self): return struct.unpack('>B', self.file.read(1))[0]
    def read_s8(self): return struct.unpack('>b', self.file.read(1))[0]
    def read_u16(self): return struct.unpack('>H', self.file.read(2))[0]
    def read_s16_le(self): return struct.unpack('<h', self.file.read(2))[0]
    def read_u32(self): return struct.unpack('>I', self.file.read(4))[0] 

def swap16(val): return ((val & 0xFF) << 8) | ((val >> 8) & 0xFF)

def unpack_triangle_indices(bitfield, permutation_flag):
    i1 = (bitfield >> 10) & 0x1F
    i2 = (bitfield >> 5) & 0x1F
    i3 = bitfield & 0x1F
    if permutation_flag == 0: return i1, i2, i3
    elif permutation_flag == 1: return i2, i3, i1
    else: return i3, i1, i2

# =============================================================================
# CLEANUP
# =============================================================================
def cleanup_scene():
    print("--- CLEANING SCENE ---")
    
    if bpy.context.active_object and bpy.context.active_object.mode != 'OBJECT':
        bpy.ops.object.mode_set(mode='OBJECT')
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()
    
    for mesh in list(bpy.data.meshes):
        if "HM64_Map" in mesh.name or "Sprite_" in mesh.name:
            bpy.data.meshes.remove(mesh)
            
    for mat in list(bpy.data.materials):
        if "Material_Tile" in mat.name or "Mat_Sprite" in mat.name:
            bpy.data.materials.remove(mat)

    # Force fresh load
    for img in list(bpy.data.images):
        if "tile-" in img.name or "map_object-" in img.name:
            bpy.data.images.remove(img)
            
    for block in bpy.data.meshes:
        if block.users == 0: bpy.data.meshes.remove(block)
    for block in bpy.data.materials:
        if block.users == 0: bpy.data.materials.remove(block)

# =============================================================================
# MAIN
# =============================================================================
def import_map():
    print("=== STARTING IMPORT ===")

    cleanup_scene()
    
    if not check_paths():
        return

    reader = BinaryReader(MAP_FILE_PATH)
    
    try:
        offsets = [reader.read_u32() for _ in range(16)]
        offset_grid = offsets[0]
        offset_mesh = offsets[1]
        offset_objects = offsets[4]
        
        print(f"Offset Grid: {offset_grid}")
        print(f"Offset Mesh: {offset_mesh}")
        
        reader.seek(offset_grid)
        reader.read_u32() # Pointer (4 bytes)
        tile_size_x = reader.read_u8()
        tile_size_z = reader.read_u8()
        map_width = reader.read_u8()
        map_height = reader.read_u8()
        
        print(f"Map Dimensions: {map_width} x {map_height}")
        print(f"Tile Size: {tile_size_x} x {tile_size_z}")
        
        if map_width == 0 or map_height == 0:
            print("Map dimensions are ZERO. Stopping.")
            return

        # gridToTileIndex is at data + 12
        grid_data_start = offset_grid + 12
        print(f"Calculated Grid Start: {grid_data_start}")
        
        mesh_data = bpy.data.meshes.new(f"HM64_Map_{MAP_NAME}")
        obj = bpy.data.objects.new(f"HM64_Map_{MAP_NAME}", mesh_data)
        bpy.context.collection.objects.link(obj)
        
        bm = bmesh.new()
        bm.from_mesh(mesh_data)
        uv_layer = bm.loops.layers.uv.new()
        color_layer = bm.loops.layers.color.new("Color")
        
        # Pre-cache texture dimensions
        texture_dims = {}
        for i in range(80):
            p = os.path.join(TEXTURES_BASE_PATH, f"tile-{i}.png")
            if os.path.exists(p):
                try:
                    tmp = bpy.data.images.load(p)
                    texture_dims[i] = (float(tmp.size[0]), float(tmp.size[1]))
                    bpy.data.images.remove(tmp)
                except: texture_dims[i] = (64.0, 64.0)
            else: texture_dims[i] = (32.0, 32.0)
            
        tiles_processed = 0
        
        for z in range(map_height):
            for x in range(map_width):
                # Explicit seek
                current_offset = grid_data_start + (z * map_width + x) * 2
                reader.seek(current_offset)
                
                swapped_index = swap16(reader.read_u16())
                
                if swapped_index == 0: 
                    continue
                
                tile_index = swapped_index - 1
                
                # Verify valid tile data
                if tiles_processed == 0:
                    print(f"First Valid Tile Found at Grid({x},{z}) -> Index {tile_index}")

                try:
                    # Jump to tile definition
                    reader.seek(offset_mesh + (tile_index * 4))
                    local_offset = reader.read_u32()
                    reader.seek(offset_mesh + local_offset)
                    
                    reader.read_u32() # Header
                    y_offset = reader.read_u8()
                    fallback_h = reader.read_u8()
                    raw_tex_index = reader.read_u8()
                    vertices_per_tile = reader.read_u8()
                    
                    texture_id = raw_tex_index & 0x7F
                    t_width, t_height = texture_dims.get(texture_id, (64.0, 64.0))

                    # Get local vertices from data 
                    local_verts = [(reader.read_s8(), reader.read_u8(), reader.read_s8()) for _ in range(vertices_per_tile)]
                    
                    # covnert to world coordinates
                    world_verts = []
                    for lv in local_verts:
                        wx = (x * tile_size_x) + lv[0]
                        wy = (y_offset + lv[1])     
                        wz = -((z * tile_size_z) + lv[2])
                        bv = bm.verts.new((wx * SCALE_FACTOR, wz * SCALE_FACTOR, wy * SCALE_FACTOR))
                        world_verts.append(bv)
                    
                    bm.verts.ensure_lookup_table()
                    
                    # Commands
                    processing_commands = True
                    tile_uvs = {}
                    
                    while processing_commands:
                        flags = reader.read_u8()
                        if flags & 0x20: 
                            reader.read_u8(); reader.read_u8()
                        
                        is_textured = (flags & 0x80)
                        is_quad = (flags & 0x40)
                        is_last_command = (flags & 0x10)
                        
                        current_solid_color = None
                        
                        if is_textured:
                            count = 4 if is_quad else 3
                            for _ in range(count):
                                v_idx = reader.read_u8()
                                s_raw = reader.read_u8()
                                t_raw = reader.read_u8()
                                
                                # corresponds to: (tileRenderingInfo[i].data2[0] << 0x16) | (tileRenderingInfo[i].data3[0] << 6)
                                u_px = s_raw * 2.0
                                v_px = t_raw * 2.0
                                
                                # gSPModifyVertex expects absolute texel coordinates (e.g., pixel 32, not float 0.5). Blender expects normalized coordinates (0.0 to 1.0).
                                # Normalize to Full Image
                                u = u_px / t_width
                                v = v_px / t_height
                                
                                # Blender Flip
                                # N64 RDP coordinate system places (0,0) at the top-left. Blender/OpenGL standard places (0,0) at the bottom-left.
                                tile_uvs[v_idx] = (u, 1.0 - v)
                        else:
                            r = reader.read_u8() / 255.0
                            g = reader.read_u8() / 255.0
                            b = reader.read_u8() / 255.0
                            current_solid_color = (r, g, b, 1.0)
                        
                        # Faces
                        t1_bits = swap16(reader.read_u16())
                        tri1_perm = (flags & 0x0C) >> 2
                        idx1, idx2, idx3 = unpack_triangle_indices(t1_bits, tri1_perm)
                        
                        def create_face(indices, is_tex, solid_col):
                            try:
                                f_verts = [world_verts[i] for i in indices]
                                face = bm.faces.new(f_verts)

                                if APPLY_TEXTURES:
                                    mat = get_material(texture_id, TEXTURES_BASE_PATH, solid_col)
                                    
                                    # Assign Material
                                    if obj.data.materials.find(mat.name) == -1:
                                        obj.data.materials.append(mat)
                                    face.material_index = obj.data.materials.find(mat.name)
                                
                                # Assign UVs
                                for i, vert_idx in enumerate(indices):
                                    loop = face.loops[i]
                                    if vert_idx in tile_uvs:
                                        loop[uv_layer].uv = tile_uvs[vert_idx]
                                    else:
                                        loop[uv_layer].uv = (0.0, 0.0)
                                    
                                    if FORCE_WHITE_VERTEX_COLORS:
                                        loop[color_layer] = (1.0, 1.0, 1.0, 1.0)
                            except ValueError:
                                pass # Duplicate face
                            except Exception as e:
                                print(f"Face Create Error: {e}")

                        create_face([idx1, idx2, idx3], is_textured, current_solid_color)
                        
                        if is_quad:
                            t2_bits = swap16(reader.read_u16())
                            tri2_perm = (flags & 0x03)
                            idx4, idx5, idx6 = unpack_triangle_indices(t2_bits, tri2_perm)
                            create_face([idx4, idx5, idx6], is_textured, current_solid_color)
                            
                        if is_last_command: 
                            processing_commands = False
                    
                    tiles_processed += 1

                except Exception as tile_err:
                    print(f"Error in tile {tile_index}: {tile_err}")
        
        print(f"Total Tiles Processed: {tiles_processed}")
        
        bm.to_mesh(mesh_data)
        bm.free()

        map_dims = (map_width, map_height, tile_size_x, tile_size_z)
        setup_camera(map_dims)
        
        if RENDER_CORE_MAP_OBJECTS:
             render_core_map_objects(reader, offset_objects, bpy.context.collection, map_dims)

        # Set view
        for area in bpy.context.screen.areas: 
            if area.type == 'VIEW_3D':
                for space in area.spaces: 
                    if space.type == 'VIEW_3D': space.shading.type = 'MATERIAL'

    except Exception as e:
        print(f"Error: {e}")
        traceback.print_exc()
    finally:
        reader.close()
        print("=== MAP IMPORT FINISHED ===")

if __name__ == "__main__":
    import_map()