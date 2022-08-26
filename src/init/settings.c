#include "../../include/main.h"


static unsigned char	load_integer(char *line, char *token, long long int *n)
{
	*n = ft_atoi(token);
	if (*n > SHRT_MAX)
	{
		ft_putendl_fd(line, 2);
		return (ERR_OUT_OF_BOUNDS_VALUE);
	}
	return (ERR_NONE);
}

static unsigned char	load_keybind(char *line, char *token, long long int *n)
{
	for (unsigned int i = 0; i < NB_KEYS; i++)
		if (ft_strcmp(gl_str_ids[i], token) == 0)
		{
			*n = i;
			return (ERR_NONE);
		}

	ft_putendl_fd(line, 2);
	return (ERR_UNRECOGNIZED_KEY_ID);
}

static unsigned char	assign_value(t_env *env, unsigned int j, char *line, char *token)
{
	long long int	n = 0;
	unsigned char	code;

	// Read value (integer)
	if (j < SET_KEY_EXIT) // if the information to load is an integer
	{
		if ((code = load_integer(line, token, &n)) != ERR_NONE)
			return (code);
	}
	else if ((code = load_keybind(line, token, &n)) != ERR_NONE)
		return (code);

	switch (j) {
	// Numerical values
		case SET_WIN_HEIGHT:
			env->settings.w_hgt = (uint16_t)n;
			break;
		case SET_WIN_WIDTH:
			env->settings.w_wdt = (uint16_t)n;
			break;
		case SET_KEY_EXIT:
			env->settings.keys[KEY_EXIT] = (uint8_t)n;
			break;
	// Key Bindings
		case SET_KEY_MOVE_CAM_FORWARD:
			env->settings.keys[KEY_MOVE_CAM_FORWARD] = (uint8_t)n;
			break;
		case SET_KEY_MOVE_CAM_BACKWARD:
			env->settings.keys[KEY_MOVE_CAM_BACKWARD] = (uint8_t)n;
			break;
		case SET_KEY_MOVE_CAM_UP:
			env->settings.keys[KEY_MOVE_CAM_UP] = (uint8_t)n;
			break;
		case SET_KEY_MOVE_CAM_DOWN:
			env->settings.keys[KEY_MOVE_CAM_DOWN] = (uint8_t)n;
			break;
		case SET_KEY_MOVE_CAM_LEFT:
			env->settings.keys[KEY_MOVE_CAM_LEFT] = (uint8_t)n;
			break;
		case SET_KEY_MOVE_CAM_RIGHT:
			env->settings.keys[KEY_MOVE_CAM_RIGHT] = (uint8_t)n;
			break;
		case SET_KEY_LIGHT:
			env->settings.keys[KEY_LIGHT] = (uint8_t)n;
			break;
		case SET_KEY_TEXTURE:
			env->settings.keys[KEY_TEXTURE] = (uint8_t)n;
			break;
	};

	return (ERR_NONE);
}

static unsigned char	loader(t_env *env, char **lines)
{
	bool			founds[SET_MAX]; // Array used to check for missing Settings.
	char			**tokens;
	unsigned char	code;
	bool			found; // Used to check if the current settings key exists.

	ft_memset(founds, 0, sizeof(bool) * SET_MAX); // No settings found yet, flat to 0.
	for (unsigned int i = 0; lines[i]; i++) // Iterate through lines.
	{
		// Split in words
		if (!(tokens = ft_strsplit(lines[i], " \b\t\v\f\r")))
			return (ERR_MALLOC_FAILED);

		// First syntax check
		if (ft_arrlen(tokens) != 3 || tokens[1][0] != '=' || tokens[1][1] != 0)
		{
			ft_putendl_fd(lines[i], 2);
			ft_arrfree(tokens);
			return (ERR_INVALID_SETTINGS_SYNTAX);
		}

		found = false;
		// Indentification of which setting the user is trying to set on this line.
		for (unsigned int j = 0; j < SET_MAX; j++)
			if (ft_strcmp(tokens[0], settings_keys[j]) == 0)
			{
				if ((code = assign_value(env, j, lines[i], tokens[2])) != ERR_NONE)
				{
					ft_arrfree(tokens);
					return (code);
				}
				found = true;
				founds[j] = true;
				break ;
			}

		if (found == false) // If identification failed.
		{
			ft_putendl_fd(lines[i], 2);
			ft_arrfree(tokens);
			return (ERR_INVALID_SETTING_KEY);
		}
		ft_arrfree(tokens);
	}

	for (unsigned int i = 0; i < SET_MAX; i++)
		if (founds[i] == false) // If any settings are missing.
		{
			ft_putendl_fd(settings_keys[i], 2);
			return (ERR_SETTING_KEY_NOT_FOUND);
		}

	return (ERR_NONE);
}

unsigned char			load_settings(t_env *env)
{
	char			**lines;
	unsigned char	code;

	// Reads file and splits it into lines
	if ((code = readlines("Settings.toml", &lines)) != ERR_NONE)
		return (code);

	if ((code = loader(env, lines)) != ERR_NONE)
	{
		ft_arrfree(lines);
		return (code);
	}

	// Free lines array
	ft_arrfree(lines);

	return (ERR_NONE);
}
