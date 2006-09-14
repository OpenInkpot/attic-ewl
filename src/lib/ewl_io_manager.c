#include <Ewl.h>
#include "ewl_private.h"
#include "ewl_debug.h"
#include "ewl_macros.h"

#include <dlfcn.h>

static Ecore_Hash *ewl_io_manager_ext_icon_map = NULL;
static int ewl_io_manager_strcasecompare(const void *key1, const void *key2);

static Ewl_IO_Manager_Plugin *ewl_io_manager_plugin_get(const char *mime);

/**
 * @return Returns TRUE on successful initialization, FALSE otherwise
 * @brief Initializes the IO manager system.
 */
int
ewl_io_manager_init(void)
{
	DENTER_FUNCTION(DLEVEL_STABLE);

	if (!ewl_io_manager_ext_icon_map)
	{
		/* XXX this is a dumb way to do this.... */
		ewl_io_manager_ext_icon_map = ecore_hash_new(ecore_str_hash,
							ewl_io_manager_strcasecompare);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".png", EWL_ICON_IMAGE_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".jpg", EWL_ICON_IMAGE_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".gif", EWL_ICON_IMAGE_X_GENERIC);

                ecore_hash_set(ewl_io_manager_ext_icon_map, ".wmv", EWL_ICON_VIDEO_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".mpg", EWL_ICON_VIDEO_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".mpeg", EWL_ICON_VIDEO_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".avi", EWL_ICON_VIDEO_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".mov", EWL_ICON_VIDEO_X_GENERIC);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".asf", EWL_ICON_VIDEO_X_GENERIC);

                ecore_hash_set(ewl_io_manager_ext_icon_map, ".mp3", EWL_ICON_AUDIO_X_GENERIC);

                ecore_hash_set(ewl_io_manager_ext_icon_map, ".html", EWL_ICON_TEXT_HTML);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".htm", EWL_ICON_TEXT_HTML);

                ecore_hash_set(ewl_io_manager_ext_icon_map, ".pl", EWL_ICON_TEXT_X_SCRIPT);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".sh", EWL_ICON_TEXT_X_SCRIPT);
                ecore_hash_set(ewl_io_manager_ext_icon_map, ".ksh", EWL_ICON_TEXT_X_SCRIPT);
	}

	DRETURN_INT(TRUE, DLEVEL_STABLE);
}

/**
 * @return Returns no value
 * @brief Shuts down the io manager system
 */
void
ewl_io_manager_shutdown(void)
{
	DENTER_FUNCTION(DLEVEL_STABLE);

	if (ewl_io_manager_ext_icon_map)
	{
		ecore_hash_destroy(ewl_io_manager_ext_icon_map);
		ewl_io_manager_ext_icon_map = NULL;
	}	

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param ext: The extension to look up
 * @return Returns the icon name for the given extension or NULL if none found
 * @brief Retrieve the icon name for the given extension or NULL if none found
 */
const char *
ewl_io_manager_extension_icon_name_get(const char *ext)
{
	char *ret = NULL;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("ext", ext, NULL);

	ret = ecore_hash_get(ewl_io_manager_ext_icon_map, ext);
	if (ret) DRETURN_PTR(ret, DLEVEL_STABLE);

	DRETURN_PTR(NULL, DLEVEL_STABLE);
}

/**
 * @param mime: The mime type to get the icon for
 * @return Returns the icon name for the given mime type or NULL if none found
 * @brief Retrives the icon name for the given mime type or NULL if none found
 */
const char *
ewl_io_manager_mime_type_icon_name_get(const char *mime)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("mime", mime, NULL);

	DWARNING("NOT WRITTEN");

	DRETURN_PTR(NULL, DLEVEL_STABLE);
}

/**
 * @param uri: The URI to look up the mime type for
 * @return Returns the mime type for the given URI or NULL on error
 * @brief Retrives the mime type for the given URI or NULL on error
 */
const char *
ewl_io_manager_uri_mime_type_get(const char *uri)
{
	char *ptr = NULL;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("uri", uri, NULL);

	/* XXX i'm cheating for now, fix this up later to use something like
	 * xdgmime */

	ptr = strrchr(uri, '.');
	if (!ptr) DRETURN_PTR(NULL, DLEVEL_STABLE);

	if (!strcmp(ptr, ".txt") || !strcmp(ptr, ".c") || !strcmp(ptr, ".h"))
	{
		DRETURN_PTR("text/plain", DLEVEL_STABLE);
	}
	else if (!strcmp(ptr, ".rtf"))
	{
		DRETURN_PTR("application/rtf", DLEVEL_STABLE);
	}
	else if (!strcmp(ptr, ".html"))
	{
		DRETURN_PTR("text/html", DLEVEL_STABLE);
	}

	DRETURN_PTR(NULL, DLEVEL_STABLE);
}

/**
 * @param uri: The URI to read
 * @return Returns a widget displaying the URI contents or NULL on error
 * @brief Creates a widget to display the URI contents. Returns NULL on error
 */
Ewl_Widget *
ewl_io_manager_uri_read(const char *uri)
{
	Ewl_Widget *ret = NULL;
	Ewl_IO_Manager_Plugin *plugin = NULL;
	const char *mime;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("uri", uri, NULL);

	mime = ewl_io_manager_uri_mime_type_get(uri);
	if (!mime)
	{
		DWARNING("Unable to determine mime type for %s", uri);
		DRETURN_PTR(NULL, DLEVEL_STABLE);
	}

	plugin = ewl_io_manager_plugin_get(mime);
	if (!plugin)
	{
		DWARNING("No plugin available to read mime type: %s.", mime);
		DRETURN_PTR(NULL, DLEVEL_STABLE);
	}

	ret = plugin->read(uri);

	DRETURN_PTR(ret, DLEVEL_STABLE);
}

/**
 * @param data: The data to write
 * @param uri: The URI to write the data too
 * @param mime: The mime type to write the data out as 
 * @return Returns no value.
 * @brief Writes the given @a data out to @a uri as mime type @a mime
 */
void
ewl_io_manager_uri_write(Ewl_Widget *data, const char *uri, const char *mime)
{
	Ewl_IO_Manager_Plugin *plugin = NULL;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("data", data);
	DCHECK_PARAM_PTR("uri", uri);
	DCHECK_PARAM_PTR("mime", mime);
	DCHECK_TYPE("data", data, EWL_WIDGET_TYPE);

	plugin = ewl_io_manager_plugin_get(mime);
	if (!plugin)
	{
		DWARNING("No plugin available to write mime type: %s.", mime);
		DRETURN(DLEVEL_STABLE);
	}

	plugin->write(data, uri);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

static
Ewl_IO_Manager_Plugin *ewl_io_manager_plugin_get(const char *mime)
{
	Ewl_IO_Manager_Plugin *plugin = NULL;
	char file[PATH_MAX];
	char *m = NULL, *ptr;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("mime", mime, NULL);

	m = strdup(mime);
	while ((ptr = strchr(m, '/')))
		*ptr = '_';

	snprintf(file, sizeof(file), "%s/plugins/ewl_io_manager_%s_plugin.so", PACKAGE_LIB_DIR, m);
	FREE(m);

	if (!ecore_file_exists(file))
		DRETURN_PTR(NULL, DLEVEL_STABLE);

	plugin = NEW(Ewl_IO_Manager_Plugin, 1);
	plugin->handle = dlopen(file, RTLD_LAZY | RTLD_GLOBAL);
	if (!plugin->handle)
	{
		FREE(plugin);
		DRETURN_PTR(NULL, DLEVEL_STABLE);
	}

	plugin->read = dlsym(plugin->handle, "ewl_io_manager_plugin_read");
	plugin->write = dlsym(plugin->handle, "ewl_io_manager_plugin_write");
	if (!plugin->read || !plugin->write)
	{
		FREE(plugin);
		DRETURN_PTR(NULL, DLEVEL_STABLE);
	}

	DRETURN_PTR(plugin, DLEVEL_STABLE);
}

static int
ewl_io_manager_strcasecompare(const void *key1, const void *key2)
{
	DENTER_FUNCTION(DLEVEL_STABLE);

	if (!key1 || !key2)
	{
		DRETURN_INT(ecore_direct_compare(key1, key2), DLEVEL_STABLE);
	}
	else if (key1 == key2)
	{
		DRETURN_INT(0, DLEVEL_STABLE);
	}

	DRETURN_INT(strcasecmp((const char *)key1,
				(const char *)key2), DLEVEL_STABLE);
}

