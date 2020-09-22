#include <string>
#include <iostream>
#include <algorithm>

#include "genomics/seq_io.hpp"

namespace genomics {
    namespace seq_io {
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
        };

        void parse_sequence(std::istream& fasta_is, std::ostream& sequence_os) {
            for(std::string line; std::getline(fasta_is, line);) {
                if (line.length() > 0 && line[0] == '>') continue;
                convert_raw_sequence(line);
                sequence_os << line;
            }
        }

        genome_structure parse_genome_structure(std::istream& fasta_is) {
            genome_structure gs;

            std::string line;
            std::getline(fasta_is, line);
            if (line.size() < 1 || line[0] != '>') {
                return gs;
            }

            line = line.substr(1);
            ltrim(line);
            rtrim(line);
            size_t length = 0;
        
            for(std::string line; std::getline(fasta_is, line); ) {
                if (line[0] == '>') {
                    chromosome c = {line, length};
                    gs.push_back(c);
                    line = line.substr(1);
                    ltrim(line);
                    rtrim(line);
                    length = 0;
                    continue;
                }

                length += line.length();
            }

            chromosome c = {line, length};
            gs.push_back(c);

            return gs;
        }

        void write_to_file(const genome_structure& gs, const std::string& filename){
            std::ofstream fs;
            fs.open(filename);
        
            for (auto p : gs) {
                fs << p.name;
                fs << "\n";
                fs << p.length;
                fs << "\n";
            }
        }

        bool load_from_file(genome_structure& gs, const std::string& filename){
            std::ifstream fs(filename);

            if (!fs) return false;
        
            while (fs) {
                std::string name, length_str;
                std::getline(fs, name);
                std::getline(fs, length_str);

                if (name.length() == 0 || length_str.length() == 0) {
                    break;
                }

                size_t length = std::stoll(length_str);
                chromosome c = {name, length};
                gs.push_back(c);
            }

            return true;
        }
    }; 
};