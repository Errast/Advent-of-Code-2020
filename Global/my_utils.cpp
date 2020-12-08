#pragma once
// this is fucking stupid
struct field_reader: std::ctype<char> {

    field_reader(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table() {
        static std::vector<std::ctype_base::mask> 
            rc(table_size, std::ctype_base::mask());

        rc['\n'] = std::ctype_base::space;
        return &rc[0];
    }
};