// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "tileset_vr4.h"

tileset_vr4_t::tileset_vr4_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, tileset_vr4_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_array = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void tileset_vr4_t::_read() {
    m_array = new std::vector<pixel_type_t*>();
    {
        int i = 0;
        while (!m__io->is_eof()) {
            m_array->push_back(new pixel_type_t(m__io, this, m__root));
            i++;
        }
    }
}

tileset_vr4_t::~tileset_vr4_t() {
    _clean_up();
}

void tileset_vr4_t::_clean_up() {
    if (m_array) {
        for (std::vector<pixel_type_t*>::iterator it = m_array->begin(); it != m_array->end(); ++it) {
            delete *it;
        }
        delete m_array; m_array = 0;
    }
}

tileset_vr4_t::pixel_type_t::pixel_type_t(kaitai::kstream* p__io, tileset_vr4_t* p__parent, tileset_vr4_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void tileset_vr4_t::pixel_type_t::_read() {
    m_color_index = m__io->read_bytes(64);
}

tileset_vr4_t::pixel_type_t::~pixel_type_t() {
    _clean_up();
}

void tileset_vr4_t::pixel_type_t::_clean_up() {
}
