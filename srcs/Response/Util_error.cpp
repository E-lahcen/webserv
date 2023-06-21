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
    r.path = getlocalPath() + "/root/error_page/500.html";
}

