meta:
  id: tileset_vr4
  endian: le
  bit-endian: le
  
seq:
  - id: array
    type: pixel_type
    repeat: eos
    doc: Graphical Data for minitiles. Referenced by VX4 or VX4EX

    
types:
  pixel_type:
    seq:
      - id: color_index
        size: 64
        doc: WPE palette color index
