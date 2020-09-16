#include "seq_io.hpp"
#include <string>
#include <iostream>
#include <algorithm>

namespace genomics::seq_io {
    namespace {
        inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
        }

        inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }

        inline char my_toupper(char ch)
        {
            return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        }

        void convert_raw_sequence(std::string& seq) {
            ltrim(seq);
            rtrim(seq);

            for (auto i = 0; i < seq.length(); i++) {
                seq[i] = toupper(seq[i]);
            }
        }
    }


    void parse_sequence(std::istream& fasta_is, std::ostream& sequence_os) {
        for(std::string line; std::getline(fasta_is, line);) {
            if (line.length() > 0 && line[0] == '>') continue;
            convert_raw_sequence(line);
            sequence_os << line;
        }
    }

    genome_structure parse_genome_structure(std::istream& fasta_is) {
        genome_structure gs;

        std::string header;
        std::getline(fasta_is, header);
        if (header.size() < 1 || header[0] != '>') {
            return gs;
        }

        header = header.substr(1);
        ltrim(header);
        rtrim(header);
        size_t offset = 0;
        
        for(std::string line; std::getline(fasta_is, line); ) {
            if (line[0] == '>') {
                gs.push_back(std::make_tuple(header, offset));
                header = line.substr(1);
                ltrim(header);
                rtrim(header);
                offset = 0;
                continue;
            }

            offset += line.length();
        }

        gs.push_back(std::make_tuple(header, offset));

        return gs;
    }

};
