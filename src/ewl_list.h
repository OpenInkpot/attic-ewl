
#ifndef __EWL_LIST_H
#define __EWL_LIST_H

struct _ewl_list {
	Ewl_Widget			widget;
	int					columns;
	char			  **titles;
	Ewd_List		  * row_data;
};

typedef struct _ewl_list Ewl_List;

#define EWL_LIST(list) ((Ewl_List *) list)

Ewl_Widget * ewl_list_new(int columns);
Ewl_Widget * ewl_list_new_all(int columns, char * titles[]);

void ewl_list_append_text(Ewl_Widget * widget, char * text[]);
void ewl_list_prepend_text(Ewl_Widget * widget, char * text[]);
void ewl_list_insert_text(Ewl_Widget * widget, char * text[], int pos);

void ewl_list_append_widgets(Ewl_Widget * widget, Ewl_Widget * widgets[]);
void ewl_list_preppend_widgets(Ewl_Widget * widget, Ewl_Widget * widgets[]);
void ewl_list_insert_widgets(Ewl_Widget * widget,
							 Ewl_Widget * widgets[],
							 int pos);

#endif
