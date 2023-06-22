#include <Response.hpp>

// calculate Length of File in Case Error
size_t Response::LengthFileError(std::string filename)
{
    size_t size;
    FILE *file = fopen(filename.c_str(), "r");
    if (file != nullptr)
    {
        if (fseek(file, 0, SEEK_END) == 0)
        {
            if(ftell(file) < 0)
            {
                fclose(file);
                return (0);
            }
            else
                size = ftell(file);
            fclose(file);
            return(size);
        }
    }
    else
    {
        fclose(file);
        return (this->error.size());
    }
    size = ftell(file);
    fclose(file);
    return(size);
}

// Error in our server
void Response::ErrorHandling(request &r)
{
    this->Status_Code = 500;
    r.SetStatutCode(500);
    r.path.clear();
}

