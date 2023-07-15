
import sys
import os

def main():
    if len(sys.argv) != 3:
        print("Usage: ", sys.argv[0], " [input spv file] [output header file]")
        return
    source_file = sys.argv[1]
    target_file = sys.argv[2]
    header_name = os.path.basename(target_file)
    header_name = os.path.splitext(header_name)[0]
    with open(target_file, 'w') as target:
        target.write("#pragma once\n#include <vector>\n\nnamespace Shader\n{\n    static const std::vector<unsigned char> ")
        target.write(header_name)
        target.write(" =\n    {\n        ")
        with open(source_file, 'rb') as source:
            byte = source.read(1)
            while byte:
                hex_value = byte.hex()
                hex_value = "0x" + hex_value
                target.write(hex_value)
                byte = source.read(1)
                if byte:
                    target.write(", ")
        target.write("\n	};\n}// namespace Shader")

if __name__ == "__main__":
    main()
