#ifndef ERROR_H
# define ERROR_H

# include <stdbool.h>

enum	e_error_id
{
	ERR_NONE,
	ERR_INVALID_ARGC,
	ERR_OPENING_FILE,
	ERR_READING_FILE,
	ERR_MALLOC_FAILED,
	ERR_OUT_OF_BOUNDS_VALUE,
	ERR_INVALID_SETTINGS_SYNTAX,
	ERR_INVALID_SETTING_KEY,
	ERR_SETTING_KEY_NOT_FOUND,
	ERR_INVALID_OBJ_FILE_EXTENSION,
	ERR_INVALID_OBJ_LINE_ID,
	ERR_INVALID_PRIMITIVE_DESCRIPTION,
	ERR_INVALID_OBJECT_NAME,
	ERR_INVALID_VERTEX_FORMAT,
	ERR_INVALID_VERTEX_TEXTURE_FORMAT,
	ERR_INVALID_VERTEX_INDEX_FOR_FACE,
	ERR_INVALID_NEWMTL_INSTRUCTION,
	ERR_INVALID_KD_INSTRUCTION,
	ERR_NO_DEFINED_MTL_FOR_KD,
	ERR_INVALID_D_INSTRUCTION,
	ERR_NO_DEFINED_MTL_FOR_D,
	ERR_INVALID_MAP_KD_INSTRUCTION,
	ERR_INCOMPATIBLE_TEXTURE_FILE,
	ERR_COULD_NOT_LOAD_BMP_FILE,
	ERR_INVALID_SYNTAX_FOR_USEMTL_INSTRUCTION,
	ERR_INVALID_NAME_FOR_USEMTL_INSTRUCTION,
	ERR_INVALID_RGB_VALUE_FOR_KD_INSTRUCTION,
	ERR_INVALID_ALPHA_VALUE,
	ERR_MTL_LINE_ID_NOT_FOUND,
	ERR_UNRECOGNIZED_KEY_ID,
	ERR_GLFW_INIT,
	ERR_FAILED_WIN,
	ERR_FAILED_GLAD,
	ERR_FAILED_TO_COMPILE_SHADER,
	ERR_UNCOMPATIBLE_OPENGL_VERSION,
	ERR_OPENING_SHADER_FILE,
	ERR_CHDIR_FAILED,
	ERR_FAILED_TO_LINK_SHADER_PROGRAM,
	ERR_PRIMARY_MONITOR_NOT_FOUND,
	ERR_INCOHERENT_FACE_INDEXES,
	ERR_INVALID_VERTEX_TEXTURE_INDEX_FOR_FACE,
	ERR_MAX
};

typedef	struct	s_error
{
	char	msg[64]; // Error message
	bool	usage; // Does usage needs to be displayed
}				t_error;

static const t_error	errors[ERR_MAX] = {
		[ERR_INVALID_ARGC] = {"Invalid number of arguments.", true},
		[ERR_OPENING_FILE] = {"Can not open file.", true},
		[ERR_OPENING_SHADER_FILE] = {"Can not open shader source file.", false},
		[ERR_READING_FILE] = {"Can not read file.", false},
		[ERR_MALLOC_FAILED] = {"malloc() call failed.", false},
		[ERR_INVALID_SETTINGS_SYNTAX] = {"Invalid syntax in Settings.toml.", false},
		[ERR_OUT_OF_BOUNDS_VALUE] = {"Out of bounds value in Settings.toml.", false},
		[ERR_INVALID_SETTING_KEY] = {"Unrecognized setting key.", false},
		[ERR_SETTING_KEY_NOT_FOUND] = {"Missing setting.", false},
		[ERR_INVALID_OBJ_LINE_ID] = {"Invalid line identifier in .obj file.", false},
		[ERR_INVALID_PRIMITIVE_DESCRIPTION] = {"Invalid face primitive description in .obj file.", false},
		[ERR_INVALID_OBJECT_NAME] = {"Invalid object declaration in .obj file.", false},
		[ERR_INVALID_VERTEX_FORMAT] = {"Invalid vertex format in .obj file.", false},
		[ERR_INVALID_VERTEX_TEXTURE_FORMAT] = {"Invalid vertex texture format in .obj file.", false},
		[ERR_INVALID_VERTEX_INDEX_FOR_FACE] = {"Invalid vertex index for face in .obj file.", false},
		[ERR_INVALID_OBJ_FILE_EXTENSION] = {"Invalid .obj file extension.", true},
		[ERR_INVALID_NEWMTL_INSTRUCTION] = {"Invalid newmtl instruction in .mtl file.", false},
		[ERR_INVALID_KD_INSTRUCTION] = {"Invalid syntax for Ka property in .mtl file.", false},
		[ERR_NO_DEFINED_MTL_FOR_KD] = {"No material defined for Ka property in .mtl file.", false},
		[ERR_INVALID_D_INSTRUCTION] = {"Invalid syntax for d property in .mtl file.", false},
		[ERR_NO_DEFINED_MTL_FOR_D] = {"No material defined for d property in .mtl file.", false},
		[ERR_INVALID_MAP_KD_INSTRUCTION] = {"Invalid syntax for Map_Kd property in .mtl file.", false},
		[ERR_INCOMPATIBLE_TEXTURE_FILE] = {"Incompatible texture file type in .mtl file.", false},
		[ERR_COULD_NOT_LOAD_BMP_FILE] = {"Could not load bmp texture file.", false},
		[ERR_INVALID_SYNTAX_FOR_USEMTL_INSTRUCTION] = {"Invalid syntax for usemtl instruction in .obj file.", false},
		[ERR_INVALID_NAME_FOR_USEMTL_INSTRUCTION] = {"Invalid name for usemtl instruction in .obj file.", false},
		[ERR_INVALID_RGB_VALUE_FOR_KD_INSTRUCTION] = {"Invalid RGB value for Kd instruction in .mtl file.", false},
		[ERR_INVALID_ALPHA_VALUE] = {"Invalid alpha value for d instruction in .mtl file.", false},
		[ERR_MTL_LINE_ID_NOT_FOUND] = {"Invalid line identifier in .mtl file.", false},
		[ERR_UNRECOGNIZED_KEY_ID] = {"Unrecognized key id in Settings.toml", false},
		[ERR_GLFW_INIT] = {"GLFW initialization failed.", false},
		[ERR_FAILED_WIN] = {"Failed to initialize window.", false},
		[ERR_FAILED_GLAD] = {"Failed to initialize GLAD.", false},
		[ERR_FAILED_TO_COMPILE_SHADER] = {"OpenGL : Failed to compile shader.", false},
		[ERR_UNCOMPATIBLE_OPENGL_VERSION] = {"Incompatible version of OpenGL.", false},
		[ERR_CHDIR_FAILED] = {"Chdir failed.", false},
		[ERR_FAILED_TO_LINK_SHADER_PROGRAM] = {"OpenGL : Failed to link shader program.", false},
		[ERR_PRIMARY_MONITOR_NOT_FOUND] = {"OpenGL : Failed to find primary monitor", false},
		[ERR_INCOHERENT_FACE_INDEXES] = {"Incoherent faces indexes in .obj file", false},
		[ERR_INVALID_VERTEX_TEXTURE_INDEX_FOR_FACE] = {"Invalid vertex texture index for face in .obj file", false}
		//[] = {"", false}
		//[] = {"", false}
};

#endif
