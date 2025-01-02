// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "lo_overlay.h"

lo_overlay_t::lo_overlay_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, lo_overlay_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_frame_pointer = 0;
    m_frame_data = 0;
    f_frame_data = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void lo_overlay_t::_read() {
    m_num_frames = m__io->read_u4le();
    m_num_overlays = m__io->read_u4le();
    m_frame_pointer = new std::vector<uint32_t>();
    const int l_frame_pointer = num_frames();
    for (int i = 0; i < l_frame_pointer; i++) {
        m_frame_pointer->push_back(m__io->read_u4le());
    }
}

lo_overlay_t::~lo_overlay_t() {
    _clean_up();
}

void lo_overlay_t::_clean_up() {
    if (m_frame_pointer) {
        delete m_frame_pointer; m_frame_pointer = 0;
    }
    if (f_frame_data) {
        if (m_frame_data) {
            for (std::vector<frame_t*>::iterator it = m_frame_data->begin(); it != m_frame_data->end(); ++it) {
                delete *it;
            }
            delete m_frame_data; m_frame_data = 0;
        }
    }
}

lo_overlay_t::frame_t::frame_t(uint8_t p_num_overlays, kaitai::kstream* p__io, lo_overlay_t* p__parent, lo_overlay_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_num_overlays = p_num_overlays;
    m_overlay = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void lo_overlay_t::frame_t::_read() {
    m_overlay = new std::vector<graphic_offset_t*>();
    const int l_overlay = num_overlays();
    for (int i = 0; i < l_overlay; i++) {
        m_overlay->push_back(new graphic_offset_t(m__io, this, m__root));
    }
}

lo_overlay_t::frame_t::~frame_t() {
    _clean_up();
}

void lo_overlay_t::frame_t::_clean_up() {
    if (m_overlay) {
        for (std::vector<graphic_offset_t*>::iterator it = m_overlay->begin(); it != m_overlay->end(); ++it) {
            delete *it;
        }
        delete m_overlay; m_overlay = 0;
    }
}

lo_overlay_t::graphic_offset_t::graphic_offset_t(kaitai::kstream* p__io, lo_overlay_t::frame_t* p__parent, lo_overlay_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void lo_overlay_t::graphic_offset_t::_read() {
    m_x_offset = m__io->read_s1();
    m_y_offset = m__io->read_s1();
}

lo_overlay_t::graphic_offset_t::~graphic_offset_t() {
    _clean_up();
}

void lo_overlay_t::graphic_offset_t::_clean_up() {
}

std::vector<lo_overlay_t::frame_t*>* lo_overlay_t::frame_data() {
    if (f_frame_data)
        return m_frame_data;
    m_frame_data = new std::vector<frame_t*>();
    const int l_frame_data = num_frames();
    for (int i = 0; i < l_frame_data; i++) {
        m_frame_data->push_back(new frame_t(num_overlays(), m__io, this, m__root));
    }
    f_frame_data = true;
    return m_frame_data;
}
