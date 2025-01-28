// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "sprites_dat.h"

sprites_dat_t::sprites_dat_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, sprites_dat_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_image = 0;
    m_health_bar = 0;
    m_unknown2 = 0;
    m_is_visible = 0;
    m_select_circle_image_size = 0;
    m_select_circle_vertical_pos = 0;
    f_num_lines = false;
    f_num_decorations = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void sprites_dat_t::_read() {
    m_image = new std::vector<uint16_t>();
    const int l_image = num_lines();
    for (int i = 0; i < l_image; i++) {
        m_image->push_back(m__io->read_u2le());
    }
    m_health_bar = new std::vector<uint8_t>();
    const int l_health_bar = (num_lines() - num_decorations());
    for (int i = 0; i < l_health_bar; i++) {
        m_health_bar->push_back(m__io->read_u1());
    }
    m_unknown2 = new std::vector<uint8_t>();
    const int l_unknown2 = num_lines();
    for (int i = 0; i < l_unknown2; i++) {
        m_unknown2->push_back(m__io->read_u1());
    }
    m_is_visible = new std::vector<uint8_t>();
    const int l_is_visible = num_lines();
    for (int i = 0; i < l_is_visible; i++) {
        m_is_visible->push_back(m__io->read_u1());
    }
    m_select_circle_image_size = new std::vector<uint8_t>();
    const int l_select_circle_image_size = (num_lines() - num_decorations());
    for (int i = 0; i < l_select_circle_image_size; i++) {
        m_select_circle_image_size->push_back(m__io->read_u1());
    }
    m_select_circle_vertical_pos = new std::vector<uint8_t>();
    const int l_select_circle_vertical_pos = (num_lines() - num_decorations());
    for (int i = 0; i < l_select_circle_vertical_pos; i++) {
        m_select_circle_vertical_pos->push_back(m__io->read_u1());
    }
}

sprites_dat_t::~sprites_dat_t() {
    _clean_up();
}

void sprites_dat_t::_clean_up() {
    if (m_image) {
        delete m_image; m_image = 0;
    }
    if (m_health_bar) {
        delete m_health_bar; m_health_bar = 0;
    }
    if (m_unknown2) {
        delete m_unknown2; m_unknown2 = 0;
    }
    if (m_is_visible) {
        delete m_is_visible; m_is_visible = 0;
    }
    if (m_select_circle_image_size) {
        delete m_select_circle_image_size; m_select_circle_image_size = 0;
    }
    if (m_select_circle_vertical_pos) {
        delete m_select_circle_vertical_pos; m_select_circle_vertical_pos = 0;
    }
}

int32_t sprites_dat_t::num_lines() {
    if (f_num_lines)
        return m_num_lines;
    m_num_lines = 386;
    f_num_lines = true;
    return m_num_lines;
}

uint8_t sprites_dat_t::num_decorations() {
    if (f_num_decorations)
        return m_num_decorations;
    m_num_decorations = 207;
    f_num_decorations = true;
    return m_num_decorations;
}
