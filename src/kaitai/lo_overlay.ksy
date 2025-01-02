meta:
  id: lo_overlay
  endian: le
  title: LO* Overlay File

seq:
  - id: num_frames
    type: u4
    doc: Number of frames in the overlay file.
    
  - id: num_overlays
    type: u4
    doc: Number of overlay images per frame in the file

  - id: frame_pointer
    type: u4
    repeat: expr
    repeat-expr: num_frames
    doc: Array of offsets for each frame.

instances:
  frame_data:
    type: frame(num_overlays)
    repeat: expr
    repeat-expr: num_frames

types:
  frame:
    params:
      - id: num_overlays
        type: u1
    seq:
      - id: overlay
        type: graphic_offset
        repeat: expr
        repeat-expr: num_overlays
    
  graphic_offset:
    seq:
      - id: x_offset
        type: s1
        doc: X offset relative to the GRP frame.

      - id: y_offset
        type: s1
        doc: Y offset relative to the GRP frame.

