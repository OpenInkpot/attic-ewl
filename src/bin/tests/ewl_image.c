#include <Ewl_Test2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static Ewl_Widget     *image_win;
static Ewl_Widget     *image_box;
static Ewl_Widget     *image;
static Ecore_DList    *images;
static Ewl_Widget     *entry_path;
static Ewl_Widget     *note_box;
static Ewl_Widget     *note;
static Ewl_Widget     *fd;

static void create_image_fd_window_response (Ewl_Widget *w, void *ev, void *data);
static void create_image_fd_cb(Ewl_Widget *w, void *ev_data, void *user_data);

static int create_test(Ewl_Container *box);
static int path_test_set_get(char *buf, int len);

static Ewl_Unit_Test image_unit_tests[] = {
		{"image path set/get", path_test_set_get},	
		{NULL, NULL}
	};

void 
test_info(Ewl_Test *test)
{
	test->name = "Image";
	test->tip = "Provides a widget for displaying evas\n"
				"loadable images, and edjes.";
	test->filename = "ewl_image.c";
	test->func = create_test;
	test->type = EWL_TEST_TYPE_SIMPLE;
	test->unit_tests = image_unit_tests;
}

static void
destroy_image_test(Ewl_Widget * w __UNUSED__, void *ev_data __UNUSED__,
						void *user_data __UNUSED__)
{
	char *str;

	ecore_dlist_goto_first(images);

	while ((str = ecore_dlist_remove_last(images)) != NULL)
		free(str);

	ecore_dlist_destroy(images);
}

static int
image_exists(char *i)
{
	struct stat st;

	if (!i || !strlen(i))
		return -1;

	if (((stat(i, &st)) == -1) || !S_ISREG(st.st_mode))
		return -1;

	return 1;
}

static void
image_goto_prev_cb(Ewl_Widget * w __UNUSED__, void *ev_data __UNUSED__,
					void *user_data __UNUSED__)
{
	char *img = NULL;

	ecore_dlist_previous(images);
	img = ecore_dlist_current(images);

	if (!img) img = ecore_dlist_goto_last(images);

	ewl_text_text_set(EWL_TEXT(entry_path), img);
	ewl_image_file_set(EWL_IMAGE(image), img, NULL);

	ewl_widget_configure(image_win);
}

static void
image_remove_cb(Ewl_Widget * w __UNUSED__, void *ev_data __UNUSED__,
					void *user_data __UNUSED__)
{
	char *img = NULL;

	img = ecore_dlist_remove(images);
	if (img)
		free(img);

	img = ecore_dlist_current(images);

	if (!img) img = ecore_dlist_goto_last(images);

	ewl_text_text_set(EWL_TEXT(entry_path), img);
	ewl_image_file_set(EWL_IMAGE(image), img, NULL);

	ewl_widget_configure(image_win);
}

static void
image_load()
{
	char *img = NULL;

	img = ewl_text_text_get(EWL_TEXT(entry_path));

	if (img && image_exists(img)) {
		ecore_dlist_append(images, img);
		ecore_dlist_goto_last(images);
		ewl_image_file_set(EWL_IMAGE(image), img, NULL);
	} else
		printf("ERROR: %s does not exist\n", img);

	ewl_widget_configure(image_win);
}

static void
image_goto_next_cb(Ewl_Widget * w __UNUSED__, void *ev_data __UNUSED__,
					void *user_data __UNUSED__)
{
	char *img = NULL;

	ecore_dlist_next(images);
	img = ecore_dlist_current(images);

	if (!img)
		img = ecore_dlist_goto_first(images);

	ewl_text_text_set(EWL_TEXT(entry_path), img);
	ewl_image_file_set(EWL_IMAGE(image), img, NULL);

	ewl_widget_configure(image_win);
}

int
create_test(Ewl_Container *box)
{
	Ewl_Widget     *scrollpane;
	Ewl_Widget     *button_hbox;
	Ewl_Widget     *button_prev, *button_remove, *button_next;
	char           *image_file = NULL;

	image_box = EWL_WIDGET(box);
	images = ecore_dlist_new();

	scrollpane = ewl_scrollpane_new();
	ewl_callback_append(scrollpane, EWL_CALLBACK_DELETE_WINDOW,
			destroy_image_test, NULL);
	ewl_container_child_append(EWL_CONTAINER(image_box), scrollpane);
	ewl_widget_show(scrollpane);

	if ((image_exists(PACKAGE_DATA_DIR "/images/e-logo.png")) != -1)
		image_file = strdup(PACKAGE_DATA_DIR "/images/e-logo.png");
	else if ((image_exists(PACKAGE_SOURCE_DIR "/data/images/e-logo.png")) != -1)
		image_file = strdup(PACKAGE_SOURCE_DIR "/data/images/e-logo.png");
	else if ((image_exists("./data/images/e-logo.png")) != -1)
		image_file = strdup("./data/images/e-logo.png");
	else if ((image_exists("../data/images/e-logo.png")) != -1)
		image_file = strdup("../data/images/e-logo.png");

	image = ewl_image_new();
	ewl_image_file_set(EWL_IMAGE(image), image_file, NULL);
	ewl_object_padding_set(EWL_OBJECT(image), 0, 0, 5, 0);
	ewl_object_alignment_set(EWL_OBJECT(image), EWL_FLAG_ALIGN_CENTER);
	ewl_container_child_append(EWL_CONTAINER(scrollpane), image);
	ewl_widget_show(image);

	if (image_file)
		ecore_dlist_append(images, image_file);

	button_hbox = ewl_hbox_new();
	ewl_box_spacing_set(EWL_BOX(button_hbox), 5);
	ewl_object_fill_policy_set(EWL_OBJECT(button_hbox),
				   EWL_FLAG_FILL_HFILL | EWL_FLAG_FILL_HSHRINK);
	ewl_container_child_append(EWL_CONTAINER(image_box), button_hbox);
	ewl_widget_show(button_hbox);

	entry_path = ewl_entry_new();
	ewl_text_text_set(EWL_TEXT(entry_path), image_file);
	ewl_object_fill_policy_set(EWL_OBJECT(entry_path),
				   EWL_FLAG_FILL_HFILL | EWL_FLAG_FILL_HSHRINK);
	ewl_object_alignment_set(EWL_OBJECT(entry_path), EWL_FLAG_ALIGN_CENTER);
	ewl_container_child_append(EWL_CONTAINER(button_hbox), entry_path);
	ewl_widget_show(entry_path);

	button_remove = ewl_button_new();
	ewl_button_label_set(EWL_BUTTON(button_remove), "Browse...");
	ewl_callback_append(button_remove, EWL_CALLBACK_CLICKED,
			    create_image_fd_cb, entry_path);
	ewl_object_fill_policy_set(EWL_OBJECT(button_remove), EWL_FLAG_FILL_NONE);
	ewl_object_alignment_set(EWL_OBJECT(button_remove),
				 EWL_FLAG_ALIGN_CENTER);
	ewl_container_child_append(EWL_CONTAINER(button_hbox), button_remove);
	ewl_widget_show(button_remove);

	button_hbox = ewl_hbox_new();
	ewl_box_spacing_set(EWL_BOX(button_hbox), 5);
	ewl_object_fill_policy_set(EWL_OBJECT(button_hbox),
				   EWL_FLAG_FILL_NONE);
	ewl_object_alignment_set(EWL_OBJECT(button_hbox),
				 EWL_FLAG_ALIGN_CENTER);
	ewl_container_child_append(EWL_CONTAINER(image_box), button_hbox);
	ewl_widget_show(button_hbox);

	button_prev = ewl_button_new();
	ewl_button_label_set(EWL_BUTTON(button_prev), "Previous");
	button_remove = ewl_button_new();
	ewl_button_label_set(EWL_BUTTON(button_remove), "Remove");
	button_next = ewl_button_new();
	ewl_button_label_set(EWL_BUTTON(button_next), "Next");

	ewl_container_child_append(EWL_CONTAINER(button_hbox), button_prev);
	ewl_container_child_append(EWL_CONTAINER(button_hbox), button_remove);
	ewl_container_child_append(EWL_CONTAINER(button_hbox), button_next);

	ewl_callback_append(button_prev, EWL_CALLBACK_CLICKED,
			    image_goto_prev_cb, NULL);
	ewl_callback_append(button_remove, EWL_CALLBACK_CLICKED,
			    image_remove_cb, NULL);
	ewl_callback_append(button_next, EWL_CALLBACK_CLICKED,
			    image_goto_next_cb, NULL);

	ewl_widget_show(button_prev);
	ewl_widget_show(button_remove);
	ewl_widget_show(button_next);


	note_box = ewl_hbox_new();
	ewl_container_child_append(EWL_CONTAINER(image_box), note_box);
	ewl_object_alignment_set(EWL_OBJECT(note_box), EWL_FLAG_ALIGN_CENTER);
	ewl_object_maximum_h_set(EWL_OBJECT(note_box), 20);
	ewl_widget_show(note_box);

	note = ewl_text_new();
	ewl_text_text_set(EWL_TEXT(note), "Simple image viewer, load up images and page through them.");
	ewl_container_child_append(EWL_CONTAINER(note_box), note);
	ewl_widget_show(note);

        ewl_widget_show(image);

	return 1;
}

static void
create_image_fd_cb(Ewl_Widget *w __UNUSED__, void *ev_data __UNUSED__,
				    void *user_data)
{
	if (fd)
		return;

	fd = ewl_filedialog_new();
	ewl_window_title_set (EWL_WINDOW (fd), "Select an Image...");
	ewl_window_name_set (EWL_WINDOW (fd), "EWL Image Test");
	ewl_window_class_set (EWL_WINDOW (fd), "EWL Filedialog");
	ewl_filedialog_type_set(EWL_FILEDIALOG(fd), EWL_FILEDIALOG_TYPE_OPEN);
	ewl_callback_append (fd, EWL_CALLBACK_VALUE_CHANGED, 
			    create_image_fd_window_response, user_data);
	ewl_widget_show(fd);
}

static void
create_image_fd_window_response (Ewl_Widget *w, void *ev, void *data)
{
	Ewl_Dialog_Event *e;
	Ewl_Widget *entry = data;
  
	e = ev;

	if (e->response == EWL_STOCK_OPEN) {
		char *path;

		printf("File open from image test: %s\n", 
		path = ewl_filedialog_file_get (EWL_FILEDIALOG (w)));
		if (path) {
			ewl_text_text_set(EWL_TEXT(entry), path);
			image_load();
			// free(path); FIXME: Is text widget allocated correctly?
		}
	}
	else {
		printf("Test program says bugger off.\n");
	}

	ewl_widget_destroy(fd);
	fd = NULL;
}

static int
path_test_set_get(char *buf, int len)
{
	Ewl_Widget *o;
	const char *t;
	int ret = 0;

	o = ewl_image_new();
	ewl_image_file_path_set(EWL_IMAGE(o), "/invalid/path");
	t = ewl_image_file_path_get(EWL_IMAGE(o));

	if (strcmp(t, "/invalid/path"))
		snprintf(buf, len, "path_get did not match path_set.");
	else
		ret = 1;

	return ret;
}