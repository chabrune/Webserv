#include "../includes/MimeUtils.hpp"

std::string MimeUtils::getTypeOfContent(const std::string &extension) {
	if (extension == "html" || extension == "javascript" || extension == "css")
		return "text";
	else if (extension == "jpg" || extension == "png")
		return "image";
	else if (extension == "mp4")
		return "video";
	std::cout << "There is an error in getTypeOfContent Function.." << std::endl;
	return "";
}
