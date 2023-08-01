#pragma once

#include <fstream>
#include <iostream>

namespace Utils {

bool readPropertyFromFile(const char* filename, const std::string& property, std::string& propertyValue)
{
    std::string line = "";
    bool found = false;
    std::ifstream file(filename);
    propertyValue = "";
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            // Skip lines that start with '#' (single-line comments)
            if (!line.empty() && line[0] == '#')
            {
                continue;
            }
            if (line.find(property + " ") == 0 || line.find(property + "=") == 0)
            {
                std::string propertyContent = line.substr(line.find("=") + 1);

                // If the property value starts with '$', recursively expand it
                if (!propertyContent.empty() && propertyContent[0] == '$')
                {
                    std::string expandedProperty = propertyContent.substr(1);
                    if (readPropertyFromFile(filename, expandedProperty, propertyValue))
                    {
                        found = true;
                        break;
                    }
                    else
                    {
                        LOGERR("Failed to find expanded property: %s", expandedProperty.c_str());
                    }
                }
                else
                {
                    // If it does not start with '$', set propertyValue directly
                    propertyValue = propertyContent;
                    if (!propertyValue.empty())
                    {
                        // Remove new line character from end of the string if it exists
                        if ((propertyValue.back() == '\r') || (propertyValue.back() == '\n'))
                        {
                            propertyValue.pop_back();
                        }
                    }
                    found = true;
                    break;
                }
            }
        }
    }
    else
    {
        LOGERR("File is not open");
    }

    // If the property was not found, set the propertyValue to an empty string
    if (!found)
    {
       LOGERR("Variable value is empty");
    }

    return found;
}

bool readFileContent(const char* filename, std::string& content)
{
    char buffer[1024];
    FILE* file = fopen(filename, "r");
    bool found = false;

    if (file)
    {
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            content += buffer;
        }
        fclose(file);
        found = true;
    }
    else
    {
        LOGERR("Failed to open the file");
    }

    return found;
}

}
