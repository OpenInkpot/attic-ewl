
#include <Ewl.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#define EWL_GLOBAL_THEMES			PACKAGE_DATA_DIR"/themes"
#define EWL_USER_THEMES				EWL_USER_DIR"/themes"

/* The path where the theme is located */
static char theme_path[PATH_LEN];

/* A global table of theme data that has the default settings */
static Ewd_Hash *def_theme_data;

static const char *theme_keys[] = {
	"/appearance/box/horizontal/base",
	"/appearance/box/horizontal/base.bits.db",
	"/appearance/box/horizontal/base/visible", "yes",
	"/appearance/box/vertical/base",
	"/appearance/box/vertical/base.bits.db",
	"/appearance/box/vertical/base/visible", "yes",

	"/appearance/button/default/base",
	"/appearance/button/default/base.bits.db",
	"/appearance/button/default/base/visible", "yes",
	"/appearance/button/default/clicked",
	"/appearance/button/default/clicked.bits.db",
	"/appearance/button/default/clicked/visible", "yes",
	"/appearance/button/default/hilited",
	"/appearance/button/default/hilited.bits.db",
	"/appearance/button/default/hilited/visible", "yes",
	"/appearance/button/default/selected",
	"/appearance/button/default/selected.bits.db",
	"/appearance/button/default/selected/visible", "yes",

	"/appearance/button/check/base-checked0",
	"/appearance/button/check/base-checked0.bits.db",
	"/appearance/button/check/base-checked0/visible", "yes",
	"/appearance/button/check/base-checked1",
	"/appearance/button/check/base-checked1.bits.db",
	"/appearance/button/check/base-checked1/visible", "yes",
	"/appearance/button/check/clicked-checked0",
	"/appearance/button/check/clicked-checked0.bits.db",
	"/appearance/button/check/clicked-checked0/visible", "yes",
	"/appearance/button/check/clicked-checked1",
	"/appearance/button/check/clicked-checked1.bits.db",
	"/appearance/button/check/clicked-checked1/visible", "yes",
	"/appearance/button/check/hilited-checked0",
	"/appearance/button/check/hilited-checked0.bits.db",
	"/appearance/button/check/hilited-checked0/visible", "yes",
	"/appearance/button/check/hilited-checked1",
	"/appearance/button/check/hilited-checked1.bits.db",
	"/appearance/button/check/hilited-checked1/visible", "yes",
	"/appearance/button/check/selected-checked0",
	"/appearance/button/check/selected-checked0.bits.db",
	"/appearance/button/check/selected-checked0/visible", "yes",
	"/appearance/button/check/selected-checked1",
	"/appearance/button/check/selected-checked1.bits.db",
	"/appearance/button/check/selected-checked1/visible", "yes",

	"/appearance/button/radio/base-checked0",
	"/appearance/button/radio/base-checked0.bits.db",
	"/appearance/button/radio/base-checked0/visible", "yes",
	"/appearance/button/radio/base-checked1",
	"/appearance/button/radio/base-checked1.bits.db",
	"/appearance/button/radio/base-checked1/visible", "yes",
	"/appearance/button/radio/clicked-checked0",
	"/appearance/button/radio/clicked-checked0.bits.db",
	"/appearance/button/radio/clicked-checked0/visible", "yes",
	"/appearance/button/radio/clicked-checked1",
	"/appearance/button/radio/clicked-checked1.bits.db",
	"/appearance/button/radio/clicked-checked1/visible", "yes",
	"/appearance/button/radio/hilited-checked0",
	"/appearance/button/radio/hilited-checked0.bits.db",
	"/appearance/button/radio/hilited-checked0/visible", "yes",
	"/appearance/button/radio/hilited-checked1",
	"/appearance/button/radio/hilited-checked1.bits.db",
	"/appearance/button/radio/hilited-checked1/visible", "yes",
	"/appearance/button/radio/selected-checked0",
	"/appearance/button/radio/selected-checked0.bits.db",
	"/appearance/button/radio/selected-checked0/visible", "yes",
	"/appearance/button/radio/selected-checked1",
	"/appearance/button/radio/selected-checked1.bits.db",
	"/appearance/button/radio/selected-checked1/visible", "yes",

	"/appearance/entry/default/base",
	"/appearance/entry/default/base.bits.db",
	"/appearance/entry/default/base/visible", "yes",
	"/appearance/entry/cursor/base",
	"/appearance/entry/cursor/base.bits.db",
	"/appearance/entry/cursor/base/visible", "yes",

	"/appearance/list/default/base",
	"/appearance/list/default/base.bits.db",
	"/appearance/list/default/base/visible", "yes",
	"/appearance/list/marker/base",
	"/appearance/list/marker/base.bits.db",
	"/appearance/list/marker/base/visible", "yes",

	"/appearance/seeker/horizontal/base",
	"/appearance/seeker/horizontal/base.bits.db",
	"/appearance/seeker/horizontal/base/visible", "yes",
	"/appearance/seeker/horizontal/dragbar",
	"/appearance/seeker/horizontal/dragbar.bits.db",
	"/appearance/seeker/horizontal/dragbar/visible", "yes",

	"/appearance/seeker/vertical/base",
	"/appearance/seeker/vertical/base.bits.db",
	"/appearance/seeker/vertical/base/visible", "yes",
	"/appearance/seeker/vertical/dragbar",
	"/appearance/seeker/vertical/dragbar.bits.db",
	"/appearance/seeker/vertical/dragbar/visible", "yes",

	"/appearance/table/default/base",
	"/appearance/table/default/base.bits.db",
	"/appearance/table/default/base/visible", "yes",

	"/appearance/window/default/base",
	"/appearance/window/default/base.bits.db",
	"/appearance/window/default/base/visible", "yes",

	NULL, NULL
};


/* Initialize the data structures involved with theme handling. This involves
 * finding the specified theme file. */
void
ewl_theme_init(void)
{
	struct stat st;
	char *str;
	char *home;

	/*
	 * Alloacte and clear the default theme 
	 */
	def_theme_data = ewd_hash_new(ewd_str_hash, ewd_str_compare);

	/*
	 * Setup a string with the path to the users theme dir 
	 */
	str = ewl_prefs_str_get("/theme/name");
	if (!str)
		DERROR("No theme name... unable to proceed.\n");

	home = getenv("HOME");
	if (!home)
		DERROR("Environment variable HOME not defined\n"
		       "Try export HOME=/home/user in a bash like environemnt or\n"
		       "setenv HOME=/home/user in a sh like environment.\n");

	snprintf(theme_path, PATH_LEN, "%s/" EWL_USER_THEMES "/%s", home,
		 str);

	/*
	 * Check the users theme dir to make sure it exists and is a dir 
	 */
	stat(theme_path, &st);
	if (!S_ISDIR(st.st_mode))
	  {

		  /*
		   * Theme dir is ok, now get the specified theme's path 
		   */
		  snprintf(theme_path, PATH_LEN, EWL_GLOBAL_THEMES "/%s",
			   str);
		  stat(theme_path, &st);

		  if (!S_ISDIR(st.st_mode))
			  DERROR("No theme dir =( exiting....");
	  }

	ewl_theme_data_set_defaults();
}

/* Initialize the widget's theme */
void
ewl_theme_init_widget(Ewl_Widget * w)
{
	DENTER_FUNCTION;
	DCHECK_PARAM_PTR("w", w);

	w->theme = def_theme_data;

	DLEAVE_FUNCTION;
}

void
ewl_theme_deinit_widget(Ewl_Widget * w)
{
	DENTER_FUNCTION;
	DCHECK_PARAM_PTR("w", w);

	/*
	 * We only want to destroy the hash if its not def_theme_data
	 * * We destroy def_theme_data from else where.. 
	 */
	if (w->theme && w->theme != def_theme_data)
		ewd_hash_destroy(w->theme);

	else
		w->theme = NULL;

	DLEAVE_FUNCTION;
}

/* Return the path of the current theme */
char *
ewl_theme_path()
{
	return strdup(theme_path);
}

/* Return the path of the current theme's fonts */
char *
ewl_theme_font_path()
{
	static char *font_path = NULL;

	DENTER_FUNCTION;

	/*
	 * No font path specified yet, so build it up 
	 */
	if (!font_path)
	  {
		  font_path = NEW(char, PATH_LEN);

		  snprintf(font_path, PATH_LEN, "%s/appearance/fonts",
			   theme_path);
	  }

	return font_path;
}

/* Return a string with the path to the specified image */
char *
ewl_theme_image_get(Ewl_Widget * w, char *k)
{
	char *path;
	char *data;
	struct stat st;

	DENTER_FUNCTION;
	DCHECK_PARAM_PTR_RET("w", w, NULL);
	DCHECK_PARAM_PTR_RET("k", k, NULL);

	data = ewl_theme_data_get(w, k);

	if (!data)
		DRETURN_PTR(NULL);


	if (!strncmp(data, "/appearance", 11))
	  {
		  path = NEW(char, PATH_LEN);
		  snprintf(path, PATH_LEN, "%s%s", theme_path, data);
		  FREE(data);
	  }
	else			/* Absolute path given, so return it */
		path = data;

	stat(path, &st);

	if (!S_ISREG(st.st_mode))
		printf("Couldn't stat %s\n", path);

	DRETURN_PTR(path);
}

/* Retrieve data from the theme */
char *
ewl_theme_data_get(Ewl_Widget * w, char *k)
{
	char *ret = NULL;
	char *v = NULL;

	DENTER_FUNCTION;
	DCHECK_PARAM_PTR_RET("k", k, NULL);

	if (w)
	  {
		  if (strlen(k) && w->theme)
		    {
			    v = ewd_hash_get(w->theme, k);

			    if (v)
				    ret = strdup(v);
		    }

		  if (!ret && strlen(k))
		    {
			    if (w->parent)
			      {
				      v = ewl_theme_data_get(w->parent, k);
				      if (v)
					      ret = v;
			      }
			    else
			      {
				      v = ewd_hash_get(def_theme_data, k);

				      if (v)
					      ret = strdup(v);
			      }
		    }
	  }
	else
	  {

		  v = ewd_hash_get(def_theme_data, k);

		  if (v)
			  ret = strdup(v);

	  }

	DRETURN_PTR(ret);
}

/* Store data into the theme */
void
ewl_theme_data_set(Ewl_Widget * w, char *k, char *v)
{
	DENTER_FUNCTION;
	DCHECK_PARAM_PTR("w", w);
	DCHECK_PARAM_PTR("k", k);

	if (w->theme == def_theme_data)
		w->theme = ewd_hash_new(ewd_str_hash, ewd_str_compare);

	ewd_hash_set(w->theme, k, v);

	ewl_widget_theme_update(w);

	DLEAVE_FUNCTION;
}

void
ewl_theme_data_set_default(char *k, char *v)
{
	DENTER_FUNCTION;

	ewd_hash_set(def_theme_data, k, v);

	DLEAVE_FUNCTION;
}

void
ewl_theme_data_gen_default_theme_db(char *f)
{
	E_DB_File *db;
	char str[7], key[512], val[512];
	int i = -1, j, jj, l;

	db = e_db_open(f);

	while (theme_keys[++i])
	  {
		  snprintf(key, 512, "%s", theme_keys[i]);

		  l = strlen(key);

		  jj = 0;

		  for (j = l - 7; j < l; j++)
			  str[jj++] = key[j];

		  if (!strncasecmp(str, "visible", 7))
			  snprintf(val, 512, "yes");
		  else
			  snprintf(val, 512, "%s.bits.db", theme_keys[i]);

		  e_db_str_set(db, key, val);

		  ++i;
	  }

	e_db_flush();

	e_db_close(db);
}

void
ewl_theme_data_set_defaults(void)
{
	char *str, *str2;
	int i;

	for (i = 0; theme_keys[i]; i++)
	  {
		  str = strdup(theme_keys[i]);
		  str2 = strdup(theme_keys[++i]);

		  ewd_hash_set(def_theme_data, str, str2);
	  }
}
