#include <Ewl.h>

/**
 * @return Returns a newly allocated row on success, NULL on failure.
 * @brief Allocate and initialize a new row
 */
Ewl_Widget *ewl_row_new()
{
	Ewl_Widget *row;

	DENTER_FUNCTION(DLEVEL_STABLE);

	row = NEW(Ewl_Row, 1);
	if (!row)
		DRETURN_PTR(NULL, DLEVEL_STABLE);

	if (!ewl_row_init(EWL_ROW(row))) {
		FREE(row);
		DRETURN_PTR(NULL, DLEVEL_STABLE);
	}

	DRETURN_PTR(row, DLEVEL_STABLE);
}

/**
 * @param row: the row object to initialize
 * @return Returns TRUE on success, FALSE on failure.
 * @brief Initialize the row fields of an inheriting object
 *
 * The fields of the @row object are initialized to their defaults.
 */
int ewl_row_init(Ewl_Row *row)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("row", row, FALSE);

	if (!ewl_container_init(EWL_CONTAINER(row), "row"))
		DRETURN_INT(FALSE, DLEVEL_STABLE);
	ewl_widget_inherit(EWL_WIDGET(row), "row");

	ewl_container_show_notify_set(EWL_CONTAINER(row), ewl_row_child_show_cb);
	ewl_container_resize_notify_set(EWL_CONTAINER(row), ewl_row_resize_cb);

	ewl_object_fill_policy_set(EWL_OBJECT(row), EWL_FLAG_FILL_HFILL);

	ewl_callback_append(EWL_WIDGET(row), EWL_CALLBACK_CONFIGURE,
			ewl_row_configure_cb, NULL);

	ewl_callback_append(EWL_WIDGET(row), EWL_CALLBACK_DESTROY,
			ewl_row_destroy_cb, NULL);

	DRETURN_INT(TRUE, DLEVEL_STABLE);
}

/**
 * @param row: the row to change the header row
 * @param header: header row for adjusting cell placement
 * @return Returns no value.
 * @brief Set the row header of constraints on cell widths
 *
 * Changes the row that cell widths and placements will be based on to @a
 * header.
 */
void
ewl_row_header_set(Ewl_Row *row, Ewl_Row *header)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("row", row);

	if (row->header == header)
		DRETURN(DLEVEL_STABLE);

	if (row->header) {
		ewl_callback_del_with_data(EWL_WIDGET(row->header),
					   EWL_CALLBACK_CONFIGURE,
					   ewl_row_header_configure_cb, row);
		ewl_callback_del_with_data(EWL_WIDGET(row->header),
					   EWL_CALLBACK_DESTROY,
					   ewl_row_header_destroy_cb, row);
	}

	row->header = header;
	if (header) {
		ewl_callback_append(EWL_WIDGET(header), EWL_CALLBACK_CONFIGURE,
				ewl_row_header_configure_cb, row);
		ewl_callback_append(EWL_WIDGET(header), EWL_CALLBACK_DESTROY,
				ewl_row_header_destroy_cb, row);

		ewl_object_fill_policy_set(EWL_OBJECT(row),
					   EWL_FLAG_FILL_HFILL);

		ewl_widget_configure(EWL_WIDGET(header));
	}
	else
		ewl_widget_configure(EWL_WIDGET(row));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param row: the row to retrieve a columns widget from
 * @param n: the column containing the desired widget
 * @return Returns widget located in column @n in @row on success.
 * @brief Retrieve the widget at a specified column
 */
Ewl_Widget *
ewl_row_column_get(Ewl_Row *row, short n)
{
	Ewl_Widget *found;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("row", row, NULL);

	found = ecore_list_goto_index(EWL_CONTAINER(row)->children, n + 1);

	DRETURN_PTR(found, DLEVEL_STABLE);
}

void
ewl_row_configure_cb(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Row *row;
	Ewl_Container *c;
	Ewl_Object *child;
	Ewl_Object *align;
	int x;

	DENTER_FUNCTION(DLEVEL_STABLE);

	row = EWL_ROW(w);
	c = EWL_CONTAINER(w);

	x = CURRENT_X(w);

	ecore_list_goto_first(c->children);

	/*
	 * This should be the common case, a row bounded by a set of fields,
	 * for forming a table.
	 */
	if (row->header) {
		int i = 0;
		int width;
		Ewl_Container *hdr;

		hdr = EWL_CONTAINER(row->header);
		align = ecore_list_goto_first(EWL_CONTAINER(hdr)->children);

		if (align) {
			x = MAX(CURRENT_X(align), CURRENT_X(w));
		}
		else
			x = CURRENT_X(w);

		while ((child = ecore_list_next(c->children))) {
			align = ecore_list_next(EWL_CONTAINER(hdr)->children);
			if (align)
				width = CURRENT_X(align) + CURRENT_W(align) - x;
			else
				width = CURRENT_W(w) /
					ecore_list_nodes(c->children);
			ewl_object_place(child, x, CURRENT_Y(w), width,
				CURRENT_H(w));
			x += width;
			i++;
		}
	}
	/*
	 * In the uncommon case, we simply try to give out a fair amount of
	 * space.
	 */
	else {
		int remains, nodes;

		remains = CURRENT_W(w);
		nodes = ecore_list_nodes(c->children);
		while ((child = ecore_list_next(c->children))) {
			int portion;

			/*
			 * Attempt to divvy up remaining space equally among
			 * remaining children.
			 */
			portion = remains / nodes; /* MAX(ewl_object_preferred_w_get(child),
					remains / nodes); */
			ewl_object_position_request(child, x, CURRENT_Y(w));
			ewl_object_w_request(child, portion);

			ewl_object_h_request(child, CURRENT_H(w));
			x = ewl_object_current_x_get(child) +
				ewl_object_current_w_get(child);

			remains -= ewl_object_current_w_get(child);
			nodes--;
		}

		if (remains > 0 && (child = ecore_list_goto_last(c->children)))
			ewl_object_w_request(child,
					ewl_object_current_w_get(child) +
					remains);
	}

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void
ewl_row_destroy_cb(Ewl_Widget * w, void *ev_data, void *user_data)
{
	DENTER_FUNCTION(DLEVEL_STABLE);

	ewl_row_header_set(EWL_ROW(w), NULL);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void
ewl_row_header_configure_cb(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Row *row;

	DENTER_FUNCTION(DLEVEL_STABLE);

	row = EWL_ROW(user_data);
	ewl_object_preferred_inner_w_set(EWL_OBJECT(w), CURRENT_W(row->header));
	ewl_widget_configure(EWL_WIDGET(row));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void
ewl_row_header_destroy_cb(Ewl_Widget * w, void *ev_data, void *user_data)
{
	Ewl_Row *row;

	DENTER_FUNCTION(DLEVEL_STABLE);

	row = EWL_ROW(user_data);
	row->header = NULL;
	ewl_row_header_set(row, NULL);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void
ewl_row_child_show_cb(Ewl_Container *c, Ewl_Widget *w)
{
	Ewl_Row *row;

	DENTER_FUNCTION(DLEVEL_STABLE);

	row = EWL_ROW(c);

	ewl_container_largest_prefer(c, EWL_ORIENTATION_VERTICAL);
	ewl_object_preferred_inner_w_set(EWL_OBJECT(c), PREFERRED_W(c) +
			ewl_object_preferred_w_get(EWL_OBJECT(w)));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

void
ewl_row_resize_cb(Ewl_Container *c, Ewl_Widget *w, int size, Ewl_Orientation o)
{
	Ewl_Row *row;

	DENTER_FUNCTION(DLEVEL_STABLE);

	row = EWL_ROW(c);
	if (o == EWL_ORIENTATION_VERTICAL)
		ewl_container_largest_prefer(c, EWL_ORIENTATION_VERTICAL);
	else
		ewl_object_preferred_inner_w_set(EWL_OBJECT(c),
				PREFERRED_W(c) + size);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}
