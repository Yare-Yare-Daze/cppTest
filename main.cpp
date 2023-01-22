#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Cell
{
public:
    std::string position;
    std::string data;

    Cell()
    {
        position = "";
        data = "";
    }

    Cell(std::string position, std::string data)
    {
        this->position = position;
        this->data = data;
    }
};

int main(int argv, char **args)
{
    std::vector<std::string> inputArgsVector;
    std::ifstream file;
    for (size_t i = 0; i < argv; i++)
    {
        inputArgsVector.push_back(args[i]);
        if (i == 1)
        {
            if(inputArgsVector[i].substr(inputArgsVector[i].find_last_of(".") + 1) == "csv")
            {
                file.open(inputArgsVector[i]);
            }
            else
            {
                std::cout << "Not supported this format!" << std::endl;
            }
        }
    }

    if (file)
    {
        std::cout << "File readed succesfull" << std::endl;
        
        std::string line;
        std::vector<std::string> linesVector;
        while (std::getline(file, line))
        {
            std::cout << line << std::endl;
            linesVector.push_back(line);
        }
        int rows = 0;
        int columns = 0;
        std::vector<std::string> allSubstrs;

        // Чтение из файла
        for (size_t i = 0; i < linesVector.size(); i++)
        {
            line = linesVector[i];
            int counterColumns = 0;
            rows++;
            while (!line.empty())
            {
                int nextPosFlag = line.find(",");
                if (nextPosFlag != std::string::npos)
                {
                    std::string substr = line.substr(0, nextPosFlag);
                    std::cout << "line: " << line << "| size: " << line.size() <<std::endl;
                    std::cout << "substr: " << substr << std::endl;
                    std::cout << "nextposflag: " << nextPosFlag << std::endl;
                    allSubstrs.push_back(substr);
                    // Для правильной обработки строки, когда в начале строки идет пустая ячейка:
                    // Например:  ,A,B
                    //            1,5,3
                    // Хотим, чтобы было так:
                    // -| A | B |
                    // 1| 5 | 3 |
                    if(substr == "") 
                    {
                        line.erase(0, 1);
                        continue;
                    }
                    // Для правильной обработки строки, когда у нас в конце строки присутсвует запятая, за которой должна идти пустая ячейка:
                    // Например: ,A,B,C
                    //           1,5,3,
                    // Хотим, чтобы было так:
                    // -| A | B | C |
                    // 1| 5 | 3 | - |
                    else if(line.size() - nextPosFlag == 1) 
                    {
                        line.erase(0, nextPosFlag);
                        continue;
                    }
                    line.erase(0, nextPosFlag + 1); // +1 для удаления запятой, стоящей за переменной
                }
                else
                {
                    std::string substr = line.substr(0, line.size());
                    allSubstrs.push_back(substr);
                    std::cout << "line: " << line << std::endl;
                    std::cout << "substr: " << substr << std::endl;
                    line.erase(0, line.size());
                }
                counterColumns++;
                if (columns < counterColumns) columns = counterColumns;
            }
        }

        std::cout << "Rows: " << rows << ", columns: " << columns << std::endl;

        std::vector<std::vector<std::string>> table2D(rows, std::vector<std::string>(columns));

        int counter = 0;
        int counterCells = 0;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                table2D[i][j] = allSubstrs[counter];
                counter++;
                if(i != 0 && j != 0)
                {
                    counterCells++;
                }
            }
        }
        std::cout << "Cells must be " << counterCells << " units" << std::endl;


        std::vector<Cell> cellVector(counterCells);
        std::cout << "Conclusion table:" << std::endl;

        std::string lastPostitonInStr = "";
        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < columns; c++)
            {
                std::cout << table2D[r][c] << "\t";
            }
            std::cout << std::endl;
        }

        
        // Присваиваем позицию по столбцам и строкам для каждого элемента вектора ячеек
        int currentRow = 0;
        int currentColumn = 1;
        for (size_t i = 0; i < cellVector.size(); i++)
        {
            if(currentColumn > columns - 1) currentColumn = 1;
            cellVector[i].position += table2D[0][currentColumn];
            currentColumn++;

            if(i % (columns - 1) == 0) currentRow++;
            cellVector[i].position += table2D[currentRow][0];
            
            if(i % (columns - 1) == 0) std::cout << std::endl;
            std::cout << i << "=" << cellVector[i].position << "\t";
        }

        std::cout << std::endl;

        counterCells = 0;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                if(i != 0 && j != 0)
                {
                    cellVector[counterCells].data = table2D[i][j];
                    counterCells++;
                }
            }
        }

        for (size_t i = 0; i < cellVector.size(); i++)
        {
            if(cellVector[i].data.find("=") != std::string::npos)
            {
                std::string cellData = cellVector[i].data;
                std::vector<size_t> operations;
                operations.push_back(cellData.find("+"));
                operations.push_back(cellData.find("-"));
                operations.push_back(cellData.find("*"));
                operations.push_back(cellData.find("/"));

                int counterOp = 0;
                for (size_t j = 0; j < operations.size(); j++)
                {
                    if(operations[j] != std::string::npos) 
                    {
                        counterOp = j;
                        break;
                    }
                }
                
                std::string operationStr = cellData.substr(operations[counterOp], 1);
                std::string valuePos1 = cellData.substr(1, operations[counterOp] - 1);
                std::string valuePos2 = cellData.substr(operations[counterOp] + 1, cellData.size() - operations[counterOp]);

                std::cout << "valuePos1: " << valuePos1 << " | valuePos2: " << valuePos2 << " | operation: " << operationStr << std::endl;
                Cell cellVal1;
                Cell cellVal2;


                for (size_t j = 0; j < cellVector.size(); j++)
                {
                    if(cellVector[j].position == valuePos1)
                    {
                        cellVal1 = cellVector[j];
                        break;
                    }
                }

                for (size_t j = 0; j < cellVector.size(); j++)
                {
                    if(cellVector[j].position == valuePos2)
                    {
                        cellVal2 = cellVector[j];
                        break;
                    }
                }

                std::cout << "cellVal1.pos: " << cellVal1.position << " | cellval1.data: " << cellVal1.data << std::endl;
                std::cout << "cellVal2.pos: " << cellVal2.position << " | cellval2.data: " << cellVal2.data << std::endl;

                int valueInt1 = std::atoi(cellVal1.data.c_str());
                int valueInt2 = std::atoi(cellVal2.data.c_str());
                int result;
                if(operationStr == "+") result = valueInt1 + valueInt2;
                else if(operationStr == "-") result = valueInt1 - valueInt2;
                else if(operationStr == "*") result = valueInt1 * valueInt2;
                else if(operationStr == "/") 
                {
                    try
                    {
                        std::cout << "Be careful to devide on zero!" << std::endl;
                        result = valueInt1 / valueInt2;
                    }
                    catch(std::runtime_error& e)
                    {
                        throw "d,vklad";
                        std::cerr << e.what() << std::endl;
                        //return -1;
                    }
                    
                    
                }
                cellVector[i].data = std::to_string(result);
                
            }
            // if(i % (columns - 1) == 0) std::cout << std::endl;
            // std::cout << cellVector[i].position << ":" << cellVector[i].data << "\t";
        }
        
        for (size_t i = 0; i < cellVector.size(); i++)
        {
            if(i % (columns - 1) == 0) std::cout << std::endl;
            std::cout << cellVector[i].position << ":" << cellVector[i].data << "\t";
        }
        
        
    }
    else
    {
        std::cout << "File not readed" << std::endl;
    }



}