#ifndef LO_OVERLAY_H_
#define LO_OVERLAY_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class lo_overlay_t : public kaitai::kstruct {

public:
    class frame_t;
    class graphic_offset_t;

    lo_overlay_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, lo_overlay_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~lo_overlay_t();

    class frame_t : public kaitai::kstruct {

    public:

        frame_t(uint8_t p_num_overlays, kaitai::kstream* p__io, lo_overlay_t* p__parent = 0, lo_overlay_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~frame_t();

    private:
        std::vector<graphic_offset_t*>* m_overlay;
        uint8_t m_num_overlays;
        lo_overlay_t* m__root;
        lo_overlay_t* m__parent;

    public:
        std::vector<graphic_offset_t*>* overlay() const { return m_overlay; }
        uint8_t num_overlays() const { return m_num_overlays; }
        lo_overlay_t* _root() const { return m__root; }
        lo_overlay_t* _parent() const { return m__parent; }
    };

    class graphic_offset_t : public kaitai::kstruct {

    public:

        graphic_offset_t(kaitai::kstream* p__io, lo_overlay_t::frame_t* p__parent = 0, lo_overlay_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~graphic_offset_t();

    private:
        int8_t m_x_offset;
        int8_t m_y_offset;
        lo_overlay_t* m__root;
        lo_overlay_t::frame_t* m__parent;

    public:

        /**
         * X offset relative to the GRP frame.
         */
        int8_t x_offset() const { return m_x_offset; }

        /**
         * Y offset relative to the GRP frame.
         */
        int8_t y_offset() const { return m_y_offset; }
        lo_overlay_t* _root() const { return m__root; }
        lo_overlay_t::frame_t* _parent() const { return m__parent; }
    };

private:
    bool f_frame_data;
    std::vector<frame_t*>* m_frame_data;

public:
    std::vector<frame_t*>* frame_data();

private:
    uint32_t m_num_frames;
    uint32_t m_num_overlays;
    std::vector<uint32_t>* m_frame_pointer;
    lo_overlay_t* m__root;
    kaitai::kstruct* m__parent;

public:

    /**
     * Number of frames in the overlay file.
     */
    uint32_t num_frames() const { return m_num_frames; }

    /**
     * Number of overlay images per frame in the file
     */
    uint32_t num_overlays() const { return m_num_overlays; }

    /**
     * Array of offsets for each frame.
     */
    std::vector<uint32_t>* frame_pointer() const { return m_frame_pointer; }
    lo_overlay_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // LO_OVERLAY_H_
