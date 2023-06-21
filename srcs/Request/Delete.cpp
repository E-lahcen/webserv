/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydahni <ydahni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 23:07:08 by ydahni            #+#    #+#             */
/*   Updated: 2023/06/21 22:42:33 by ydahni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>


int request::RemoveDirectory(std::string path)
{
    if (access(path.c_str(), F_OK | W_OK | X_OK ) == 0)
    {
        DIR* d = opendir(path.c_str());
        struct dirent* type;
        while ((type = readdir(d)) != NULL)
        {
            if (type->d_type == DT_REG)
            {
                std::string file = JoinePathToRoot(path , type->d_name);
                int error = RemoveFile(file);
                if (error == 409)
                {
                    closedir(d);
                    return (error);
                }
            }
            else if (type->d_type == DT_DIR)
            {
                std::string directory = type->d_name;
                if (directory == "." || directory == "..")
                    continue;
                else
                {
                    directory = JoinePathToRoot(path , type->d_name);
                    int error = RemoveDirectory(JoinePathToRoot(path , type->d_name));
                    if (error == 409)
                    {
                        closedir(d);
                        return (error);
                    }
                }
            }
        }
        closedir(d);
    }
    else
        return (409);
    if (rmdir(path.c_str()) != 0)
        return (409);
    return (204);
}


int request::RemoveFile(std::string file)
{
    if (access(file.c_str(), F_OK | W_OK ) == 0)
    {
        if (remove(file.c_str()) != 0)
            return (409);
    }
    else
        return (409);
    return (0);
}


void request::Delete()
{
    struct stat info;
    if (stat(this->path.c_str(), &info) == 0)
    {
        if (S_ISDIR(info.st_mode) == 1 && this->StatutCode == 0)
        {
            if (this->uri[this->uri.size() -1] == '/')
            {
                if (access(this->path.c_str(), F_OK | W_OK | X_OK ) == 0)
                {
                    int error = RemoveDirectory(this->path);
                    if (error == 204)
                    {
                        SetStatutCode(204);
                        this->finishRead = 0;
                        this->path.clear();
                    }
                    else
                        CheckErrorsPage(error);
                }
                else
                    CheckErrorsPage(403);
            }
            else
                CheckErrorsPage(409);
                
        }
        if(S_ISREG(info.st_mode) == 1 && this->StatutCode == 0)
        {
            if (access(this->path.c_str(), F_OK | W_OK ) == 0)
            {
                int error = RemoveFile(this->path);
                if (error == 0)
                {
                    SetStatutCode(204);
                    this->finishRead = 0;
                }
                else
                    CheckErrorsPage(error);
            }
            else
                CheckErrorsPage(403);
        }
           
    }
    else
        CheckErrorsPage(404);
}