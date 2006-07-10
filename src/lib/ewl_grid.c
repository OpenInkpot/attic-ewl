#include "Ewl.h"
#include "ewl_debug.h"
#include "ewl_macros.h"
#include "ewl_private.h"

static void ewl_grid_resize(Ewl_Grid *g);
static void ewl_grid_map_recalc(Ewl_Grid *g);
static void ewl_grid_child_data_collect(Ewl_Grid *g);
static void ewl_grid_hmap_position_next(Ewl_Grid *g, int *c, int *r);
static void ewl_grid_vmap_position_next(Ewl_Grid *g, int *c, int *r);
static void ewl_grid_map_start_position_get(Ewl_Grid *g, int *c, int *r);

/**
 * @param cols: number of columns
 * @param rows: number of rows
 * @return Returns a pointer to a grid on success or NULL on failure.
 * @brief Create a new Ewl_Grid widget
 */
Ewl_Widget *
ewl_grid_new(void)
{
	Ewl_Grid *g;

	DENTER_FUNCTION(DLEVEL_STABLE);

	g = NEW(Ewl_Grid, 1);
	if (!g)
		DRETURN_PTR(NULL, DLEVEL_STABLE);

	if (!ewl_grid_init(g)) {
		ewl_widget_destroy(EWL_WIDGET(g));
		g = NULL;
	}

	DRETURN_PTR(EWL_WIDGET(g), DLEVEL_STABLE);
}

/**
 * @param g: the grid
 * @param cols: number of columns
 * @param rows: number of rows
 * @return Returns no value. Responsible for setting up default values and
 * callbacks within a grid structure
 * @brief Initializes an Ewl_Grid widget to default values
 */
int
ewl_grid_init(Ewl_Grid *g)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("g", g, FALSE);

	/*
	 * Initialize the grids inherited fields
	 */
	if (!ewl_container_init(EWL_CONTAINER(g)))
		DRETURN_INT(FALSE, DLEVEL_STABLE);

	ewl_widget_appearance_set(EWL_WIDGET(g), EWL_GRID_TYPE);
	ewl_widget_inherit(EWL_WIDGET(g), EWL_GRID_TYPE);

	ewl_container_add_notify_set(EWL_CONTAINER(g), ewl_grid_child_add_cb);
	ewl_container_remove_notify_set(EWL_CONTAINER(g), 
						ewl_grid_child_remove_cb);
	ewl_container_resize_notify_set(EWL_CONTAINER(g),
						ewl_grid_child_resize_cb);

	/*
	 * the smallest size where a grid make sense
	 */
	ewl_grid_dimensions_set(g, 2, 2);
	
	g->homogeneous_h = FALSE;
	g->homogeneous_v = FALSE;

	g->orientation = EWL_ORIENTATION_HORIZONTAL;

	/*
	 * Append callbacks
	 */
	ewl_callback_append(EWL_WIDGET(g), EWL_CALLBACK_CONFIGURE,
					ewl_grid_configure_cb, NULL);
	ewl_callback_prepend(EWL_WIDGET(g), EWL_CALLBACK_DESTROY, 
					ewl_grid_destroy_cb, NULL);

	ewl_widget_focusable_set(EWL_WIDGET(g), FALSE);

	DRETURN_INT(TRUE, DLEVEL_STABLE);
}

/**
 * @param g: the grid
 * @param cols: the new number of columns
 * @param rows: the new number of rows
 * @return Returns no value
 * @brief Clears the grid and sets new geometry
 */
void
ewl_grid_dimensions_set(Ewl_Grid *g, int cols, int rows)
{
	Ewl_Widget *w;
	Ewl_Grid_Info *col_new, *row_new;
	int i;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	w = EWL_WIDGET(g);

	if ((cols == g->cols) && (rows == g->rows))
		DRETURN(DLEVEL_STABLE);
	
	IF_FREE(g->map);
	g->map = NULL;
	g->data_dirty = TRUE;
	
	if (cols != g->cols) {
		col_new = NEW(Ewl_Grid_Info, cols);
		if (!col_new)
			DRETURN(DLEVEL_STABLE);
		
		if (g->col_size) {
			int num;
			
			num = MIN(cols, g->cols);
			for (i = 0; i < num; i++) 
				col_new[i] = g->col_size[i];

			FREE(g->col_size);
		}
		g->col_size = col_new;
		g->cols = cols;
	}

	if (rows != g->rows) {
		row_new = NEW(Ewl_Grid_Info, rows);
		if (!row_new)
			DRETURN(DLEVEL_STABLE);
		
		if (g->row_size) {
			int num;
			
			num = MIN(rows, g->rows);
			for (i = 0; i < num; i++) 
				row_new[i] = g->row_size[i];

			FREE(g->row_size);
		}
		g->row_size = row_new;
		g->rows = rows;
	}

	ewl_widget_configure(w);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid to change the fill orientation 
 * @param h: the Ewl_Orientation value
 * @return Returns no value.
 * @brief Change the fill orientation
 */
void
ewl_grid_orientation_set(Ewl_Grid *g, Ewl_Orientation orientation)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	if (g->orientation == orientation)
		DRETURN(DLEVEL_STABLE);

	g->orientation = orientation;
	g->data_dirty = TRUE;
		
	ewl_widget_configure(EWL_WIDGET(g));
	
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid to get the fill orientation
 * @return The fill orientation flag 
 * @brief Retrieves the fill orientation flag  
 */
Ewl_Orientation 
ewl_grid_orientation_get(Ewl_Grid *g)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("g", g, 0);
	DCHECK_TYPE_RET("g", g, EWL_GRID_TYPE, 0);

	DRETURN_INT(g->orientation, DLEVEL_STABLE); 
}

/**
 * @param g: the grid to change homogeneous layout 
 * @param h: the boolean value to change the layout mode to
 * @return Returns no value.
 * @brief Change the homogeneous layout of the grid
 *
 * Grids use non-homogeneous layout by default, this can be used
 * to change that. 
 */
void
ewl_grid_homogeneous_set(Ewl_Grid *g, unsigned int h)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

 	if (g->homogeneous_h != h)
		ewl_grid_hhomogeneous_set(g, h);

	if (g->homogeneous_v != h)
		ewl_grid_vhomogeneous_set(g, h);
	
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid to change horizontal homogeneous layout 
 * @param h: the boolean value to change the horizontal layout mode to
 * @return Returns no value.
 * @brief Change the horizontal homogeneous layout of the box
 *
 * Grids use non-homogeneous layout by default, this can be used
 * to change that for horizontal orientation, i.e. all columns can
 * have the same width. 
 */
void
ewl_grid_hhomogeneous_set(Ewl_Grid *g, unsigned int h)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	if (g->homogeneous_h == h)
		DRETURN(DLEVEL_STABLE);

	g->homogeneous_h = h; 
	g->data_dirty = TRUE;
	ewl_widget_configure(EWL_WIDGET(g));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid to change vertical homogeneous layout 
 * @param h: the boolean value to change the vertical layout mode to
 * @return Returns no value.
 * @brief Change the vertical homogeneous layout of the box
 *
 * Grids use non-homogeneous layout by default, this can be used
 * to change that for vertical orientation, i.e. all rows can have 
 * the same height. 
 */
void
ewl_grid_vhomogeneous_set(Ewl_Grid *g, unsigned int h)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

 	if (g->homogeneous_v == h)
		DRETURN(DLEVEL_STABLE);

	g->homogeneous_v = h; 
	g->data_dirty = TRUE;
	ewl_widget_configure(EWL_WIDGET(g));
	
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid to get the homogeneous layout 
 * @return The horizontal homogeneous flag 
 * @brief Retrieves the horizontal homogeneous flag  
 */
unsigned int 
ewl_grid_hhomogeneous_get(Ewl_Grid *g)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("g", g, 0);
	DCHECK_TYPE_RET("g", g, EWL_GRID_TYPE, 0);

	DRETURN_INT(g->homogeneous_h, DLEVEL_STABLE); 
}

/**
 * @param g: the grid to get the vertical layout 
 * @return The vertical homogeneous flag 
 * @brief Retrieves the vertical homogeneous flag  
 */
unsigned int
ewl_grid_vhomogeneous_get(Ewl_Grid *g)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR_RET("g", g, 0);
	DCHECK_TYPE_RET("g", g, EWL_GRID_TYPE, 0);

	DRETURN_INT(g->homogeneous_v, DLEVEL_STABLE); 
}

/**
 * @param g: the grid
 * @param w: the child widget
 * @param start_col: the start column
 * @param end_col: the end column
 * @param start_row: the start row
 * @param end_row: the end row
 * @return Returns no value
 * @brief Give a child widget a fixed-postion in the grid
 */
void
ewl_grid_child_position_set(Ewl_Grid *g, Ewl_Widget *w,
				int start_col, int end_col, 
				int start_row, int end_row)
{
	Ewl_Grid_Child *child;
	int new_cols, new_rows;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_PARAM_PTR("w", w);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);
	DCHECK_TYPE("w", w, EWL_WIDGET_TYPE);

	IF_FREE(g->map);
	g->map = NULL;
	g->data_dirty = TRUE;

	/*
	 * check bounds
	 */
	if (start_col < 1) {
		DWARNING("start_col out of bounds. min is 1\n");
		DLEAVE_FUNCTION(DLEVEL_STABLE);
	}

	if (end_col > g->cols)
		new_cols = end_col;
	else
		new_cols = g->cols;
	
	if (start_row < 1) {
		DWARNING("start_row out of bounds. min is 1\n");
		DLEAVE_FUNCTION(DLEVEL_STABLE);
	}

	if (end_row > g->rows)
		new_rows = end_row;
	else
		new_rows = g->rows;

	/* 
	 * create a new child 
	 */
	child = NEW(Ewl_Grid_Child, 1);
	if (!child)
		DLEAVE_FUNCTION(DLEVEL_STABLE);

	child->start_col = start_col;
	child->end_col = end_col;
	child->start_row = start_row;
	child->end_row = end_row;

	g->space += (end_col - start_col + 1) * (end_row - start_row + 1) - 1;
	
	/*
	 * if there is to less space resize the grid
	 */
	if (g->space > (new_cols * new_rows)) {
		if (g->orientation == EWL_ORIENTATION_HORIZONTAL)
			new_rows = g->space / new_cols + 1;
		else
			new_cols = g->space / new_rows + 1;
	}
	
	/* 
	 * store the child info in the child widget 
	 */
	ewl_widget_data_set(w, (void *)g, child);
	ewl_grid_dimensions_set(g, new_cols, new_rows);

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid
 * @param col: the column
 * @param relw: width relative to the grid width
 * @param width: the new width
 * @return Returns no value.
 * @brief Set the widget of a column
 */
void
ewl_grid_column_w_set(Ewl_Grid *g, int col, float relw, int width)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	/*
	 * check bounds
	 */
	if (col < 1) {
		DWARNING("parameter 'col' is out of bounds\n");
		DLEAVE_FUNCTION(DLEVEL_STABLE);
	}
	else if (col > g->cols) {
		ewl_grid_dimensions_set(g, col, g->rows);
	}

	g->col_size[col - 1].override = 1;
	g->col_size[col - 1].size = width;
	g->col_size[col - 1].rel_size = relw;
	g->data_dirty = TRUE;

	ewl_widget_configure(EWL_WIDGET(g));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid
 * @param col: the column
 * @param relw: the width relative to the grid width
 * @param width: integer pointer to store the width in
 * @return Returns no value.
 * @brief Get the user set width of a column
 *
 * This function returns only the size set by the user.
 */
void
ewl_grid_column_w_get(Ewl_Grid *g, int col, float *relw,  int *width)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_PARAM_PTR("width", width);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	if ((col > g->cols) || (col < 1)) {
		if (width) *width = 0;
		if (relw) *relw = 0.0;
	}
	else {
		if (width) *width = g->col_size[col-1].size;
		if (relw) *relw = g->col_size[col-1].rel_size;
	}

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid
 * @param row: the row
 * @param relh: the height relative to the size of the grid
 * @param height: the new height
 * @return Returns no value.
 * @brief Set the user set height of a row
 */
void
ewl_grid_row_h_set(Ewl_Grid *g, int row, float relh, int height)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	/* check bounds */
	if (row < 1) {
		DLEAVE_FUNCTION(DLEVEL_STABLE);
	}
	else if (row > g->rows) {
		ewl_grid_dimensions_set(g, g->cols, row);
	}

	g->row_size[row - 1].override = 1;
	g->row_size[row - 1].size = height;
	g->row_size[row - 1].rel_size = relh;

	ewl_widget_configure(EWL_WIDGET(g));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @param g: the grid
 * @param row: the row
 * @param relh: the height relative to the size of the grid
 * @param height: integer pointer to store the height in
 * @return Returns no value.
 * @brief Get the user set height of a row
 * 
 * This function returns only the size set by the user.
 */
void
ewl_grid_row_h_get(Ewl_Grid *g, int row, float *relh, int *height)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_PARAM_PTR("height", height);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	if ((row > g->rows) || (row < 1)) {
		if (height) *height = 0;
		if (relh) *relh = 0.0;
	}
	else {
		if (height) *height = g->row_size[row - 1].size;
		if (relh) *relh = g->row_size[row - 1].rel_size;
	}

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param w: The widget to work with
 * @param ev_data: UNUSED
 * @param user_data: UNUSED
 * @return Returns no value
 * @brief The configure callback
 */
void
ewl_grid_configure_cb(Ewl_Widget *w, void *ev_data __UNUSED__,
					void *user_data __UNUSED__)
{
	Ewl_Grid *g;
	Ewl_Grid_Child *c;
	Ewl_Widget *child;
	int c_w = 0, c_h = 0;	/* child width/height */
	int c_x = 0, c_y = 0;	/* child x/y coordinate */
	int col, row;
	int i = 0;
	void (*go_next)(Ewl_Grid *g, int *c, int *r);
	
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("w", w);
	DCHECK_TYPE("w", w, EWL_WIDGET_TYPE);

	g = EWL_GRID(w);

	if (!g->map)
		ewl_grid_map_recalc(g);

	if (g->data_dirty)
		ewl_grid_child_data_collect(g);
	
	ewl_grid_resize(g);

	/*
	 * setup the position stuff for the floating
	 * widgets
	 */
	ewl_grid_map_start_position_get(g, &col, &row);
	if (g->orientation == EWL_ORIENTATION_HORIZONTAL)
		go_next = ewl_grid_hmap_position_next;
	else
		go_next = ewl_grid_vmap_position_next;

	ecore_dlist_goto_first(EWL_CONTAINER(w)->children);
	while ((child = ecore_dlist_next(EWL_CONTAINER(w)->children))) {
		/* 
		 * reset geometry values for the next child 
		 */
		c_x = CURRENT_X(EWL_OBJECT(w));
		c_y = CURRENT_Y(EWL_OBJECT(w));
		c_w = 0;
		c_h = 0;

		c = (Ewl_Grid_Child *)ewl_widget_data_get(child, (void *) g);
		if (c) {
			/*
			 * calculate the geometry of the fixed widgets
			 */
			
			/* calculate child widgets width */
			for (i = (c->start_col - 1); i < c->end_col; i++)
				c_w += g->col_size[i].current_size;

			/* calculate child widgets height */
			for (i = (c->start_row - 1); i < c->end_row; i++)
				c_h += g->row_size[i].current_size;

			/* calculate child widgets x coordinate */
			for (i = 0; i < (c->start_col - 1); i++)
				c_x += g->col_size[i].current_size;

			/* calculate child widgets y coordinate */
			for (i = 0; i < (c->start_row - 1); i++)
				c_y += g->row_size[i].current_size;
		}
		else {
			/* 
			 * get the geometry of the non-fixed widgets
			 */
			c_w = g->col_size[col].current_size;
			c_h = g->row_size[row].current_size;
			
			/* calculate child widgets x coordinate */
			for (i = 0; i < col; i++)
				c_x += g->col_size[i].current_size;

			/* calculate child widgets y coordinate */
			for (i = 0; i < row; i++)
				c_y += g->row_size[i].current_size;
			
			go_next(g, &col, &row);
		}

		ewl_object_place(EWL_OBJECT(child), c_x, c_y, c_w, c_h);
		ewl_widget_configure(child);
	}

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param w: The widget to work with
 * @param ev_data: UNUSED
 * @param user_data: UNUSED
 * @return Returns no value
 * @brief The destroy callback
 */
void
ewl_grid_destroy_cb(Ewl_Widget *w, void *ev_data __UNUSED__,
				void *user_data __UNUSED__)
{
	Ewl_Grid *g;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("w", w);
	DCHECK_TYPE("w", w, EWL_WIDGET_TYPE);

	g = EWL_GRID(w);

	IF_FREE(g->map);
	IF_FREE(g->col_size)
	IF_FREE(g->row_size)
	g->col_size = NULL;
	g->row_size = NULL;
	g->map = NULL;

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param g: The grid to work with
 * @return Returns no value
 * @brief set up the map of the fixed postioned children
 */
static void
ewl_grid_map_recalc(Ewl_Grid *g)
{	
	Ewl_Widget *child;
	Ewl_Grid_Child *c;
	int l, k;
	
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	IF_FREE(g->map);
	g->map = NEW(char, g->cols * g->rows);
	
	ecore_dlist_goto_first(EWL_CONTAINER(g)->children);
	while ((child = ecore_dlist_next(EWL_CONTAINER(g)->children))) {
		c = (Ewl_Grid_Child *) ewl_widget_data_get(child, (void *)g);
		if (!c) continue;

		/* 
		 * mark all positions that content a  positioned widget 
		 */
		for (l = (c->start_col - 1); l < c->end_col && l < g->cols; l++)
			for (k = c->start_row - 1; k < c->end_row && k < g->rows; k++)
				g->map[g->cols * k + l]  = 1;
	}
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param g: The grid to work with
 * @return Returns no value
 * @brief collect the preferred sizes of the columns and rows
 */
static void
ewl_grid_child_data_collect(Ewl_Grid *g)
{
	int col, row;
	Ewl_Grid_Child *c;
	Ewl_Widget *child;
	void (*go_next)(Ewl_Grid *g, int *c, int *r);
	
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	/*
	 * setup the position stuff for the floating
	 * widgets
	 */
	ewl_grid_map_start_position_get(g, &col, &row);
	if (g->orientation == EWL_ORIENTATION_HORIZONTAL)
		go_next = ewl_grid_hmap_position_next;
	else
		go_next = ewl_grid_vmap_position_next;

	/*
	 * First collect the data of the non-fixed postion widgets
	 */
	ecore_dlist_goto_first(EWL_CONTAINER(g)->children);
	while ((child = ecore_dlist_next(EWL_CONTAINER(g)->children))) {
		int pref_w, pref_h;

		c = (Ewl_Grid_Child *) ewl_widget_data_get(child, (void *) g);
		if (c) continue;


		/*
		 * go to the next free place
		 */
		pref_w = ewl_object_preferred_w_get(EWL_OBJECT(child));
		pref_h = ewl_object_preferred_h_get(EWL_OBJECT(child));
		g->col_size[col].preferred_size = 
				MAX(g->col_size[col].preferred_size, pref_w);
		g->row_size[row].preferred_size = 
				MAX(g->row_size[row].preferred_size, pref_h);

		go_next(g, &col, &row);
	}

	/*
	 *  and now collect the data of the fixed postion widgets
	 */
	ecore_dlist_goto_first(EWL_CONTAINER(g)->children);
	while ((child = ecore_dlist_next(EWL_CONTAINER(g)->children))) {
		int pref_w = 0, pref_h = 0;
		int i;

		c = (Ewl_Grid_Child *) ewl_widget_data_get(child, (void *) g);
		if (!c) continue;


		/* 
		 * first calculate the current preferred size 
		 * of the cells 
		 */
		for (i = (c->start_col - 1); i < c->end_col; i++)
			pref_w += g->col_size[i].preferred_size;

		for (i = (c->start_row - 1); i < c->end_row; i++)
			pref_h += g->row_size[i].preferred_size;
			
		pref_w = ewl_object_preferred_w_get(EWL_OBJECT(child)) - pref_w;
		pref_h = ewl_object_preferred_h_get(EWL_OBJECT(child)) - pref_h;
			
		if (pref_w > 0) {
			pref_w /= c->end_col - c->start_col + 1;
			for (i = (c->start_col - 1); i < c->end_col; i++)
				g->col_size[i].preferred_size += pref_w;
		}

		if (pref_h > 0) {
			pref_h /= c->end_row - c->start_row + 1;
			for (i = (c->start_row - 1); i < c->end_row; i++)
				g->row_size[i].preferred_size += pref_h;
		}
	}

	/*
	 * calculate the preferred size
	 */

	if (g->homogeneous_h) {
		int i, size;

		for (i = 0, size = 0; i < g->cols; i++) {
			size = MAX(size, g->col_size[i].preferred_size);
		}
		ewl_object_preferred_inner_w_set(EWL_OBJECT(g), size * g->cols);
	}
	else {
		float rel;
		int i, fixed;
		
		rel = 0.0;
		fixed = 0;
		for (i = 0; i < g->cols; i++) {
			if (g->col_size[i].override) {
				rel += g->col_size[i].rel_size;
				fixed += g->col_size[i].size;
			}
			else
				fixed += g->col_size[i].preferred_size;
		}
		ewl_object_preferred_inner_w_set(EWL_OBJECT(g), 
						(int)(fixed / (1.0 - rel)));
	}
	
	if (g->homogeneous_v) {
		int i, size;

		for (i = 0, size = 0; i < g->rows; i++)
			size = MAX(size, g->row_size[i].preferred_size);

		ewl_object_preferred_inner_h_set(EWL_OBJECT(g), size * g->rows);
	}
	else {
		float rel;
		int i, fixed;
		
		rel = 0.0;
		fixed = 0;
		for (i = 0; i < g->rows; i++) {
			if (g->row_size[i].override) {
				rel += g->row_size[i].rel_size;
				fixed += g->row_size[i].size;
			}
			else
				fixed += g->row_size[i].preferred_size;
		}
		ewl_object_preferred_inner_h_set(EWL_OBJECT(g), 
						(int)(fixed / (1.0 - rel)));
	}
	
	g->data_dirty = FALSE;

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param g: The grid to work with
 * @return Returns no value
 * @brief recalculate the current size of the columns and rows
 */
static void
ewl_grid_resize(Ewl_Grid *g)
{
	int i, new_w = 0, new_h = 0;
	int left_over, left_over2;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	new_w = CURRENT_W(g);
	new_h = CURRENT_H(g);

	/* 
	 * calculated the new columns' widths
	 */
	if (g->homogeneous_h) {
		for (i = 0; i < g->cols; i++)
			g->col_size[i].current_size = new_w / g->cols;
	}
	else {
		int pref_size, fixed_size;
		double rel;

		pref_size = fixed_size = 0;
		/* 
		 * first we calculate the preferred size 
		 */
		for (i = 0; i < g->cols; i++) {
			if (!g->col_size[i].override)
				pref_size += 
					g->col_size[i].current_size 
					= g->col_size[i].preferred_size;
			else 
				fixed_size += 
					g->col_size[i].current_size
					= g->col_size[i].size
					+ g->col_size[i].rel_size 
					* new_w;
		}

		/*
		 * we can only distribute the rest size to the 
		 * non-fixed ones
		 */
		rel = (double)(new_w - fixed_size) / (double)pref_size;
		for (i = 0; i < g->cols; i++)
			if (!g->col_size[i].override)
				g->col_size[i].current_size = 
					(int)g->col_size[i].current_size * rel;
	}

	/*
	 * calculated the new rows' heights
	 */
	if (g->homogeneous_v) {
		for (i = 0; i < g->rows; i++)
			g->row_size[i].current_size = new_h / g->rows;
	}
	else {
		int pref_size, fixed_size;
		double rel;

		pref_size = fixed_size = 0;
		/* 
		 * first we calculate the preferred size 
		 */
		for (i = 0; i < g->rows; i++) {
			if (!g->row_size[i].override)
				pref_size += 
					g->row_size[i].current_size 
					= g->row_size[i].preferred_size;
			else 
				fixed_size += 
					g->row_size[i].current_size
					= g->row_size[i].size
					+ g->row_size[i].rel_size 
					* new_h;
		}
		/*
		 * we can only distribute the rest size 
		 * to the non-fixed ones
		 */
		rel = (double)(new_h - fixed_size) / (double)pref_size;
		for (i = 0; i < g->rows; i++)
			if (!g->row_size[i].override)
				g->row_size[i].current_size = 
					g->row_size[i].current_size * rel;
	}

	/*
	 * since the above set values may be doubles rounded down there
	 * might be some more space to fill at the right and bottom.
	 * this claims the left over space
	 */
	left_over = new_w;
	for (i = 0; i < g->cols; i++)
		left_over -= g->col_size[i].current_size;

	if (g->cols == 0)
		g->cols = 1;

	while (left_over != 0) {
		if (left_over > 0) {
			g->col_size[left_over % g->cols].current_size += 1;
			left_over--;
		} else if (left_over < 0) {
			left_over2 = 0 - left_over;
			g->col_size[left_over2 % g->cols].current_size -= 1;
			left_over++;
		}
	}

	left_over = new_h;
	for (i = 0; i < g->rows; i++)
		left_over -= g->row_size[i].current_size;

	if (g->rows == 0)
		g->rows = 1;

	while (left_over != 0) {
		if (left_over > 0) {
			g->row_size[left_over % g->rows].current_size += 1;
			left_over--;
		} else if (left_over < 0) {
			left_over2 = 0 - left_over;
			g->row_size[left_over2 % g->rows].current_size -= 1;
			left_over++;
		}
	}

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param g: The grid to work with
 * @param c: a pointer to the current column to get the next position
 * @param r: a pointer to the current row to get the next position
 * @return Returns no value
 * @brief get the next free position for floating children
 *
 * Use this function in the horizontal fill orientation case.
 */
static void
ewl_grid_hmap_position_next(Ewl_Grid *g, int *c, int *r)
{
	int col, row;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_PARAM_PTR("c", c);
	DCHECK_PARAM_PTR("r", r);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);
	
	col = *c;
	row = *r;
	
	do {
		col++;
		if (col >= g->cols) {
			col = 0;
			row++;
		}
	} while (g->map[col + (row * g->cols)] != 0);
	
	*c = col;
	*r = row;
	
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param g: The grid to work with
 * @param c: a pointer to the current column to get the next position
 * @param r: a pointer to the current row to get the next position
 * @return Returns no value
 * @brief get the next free position for floating children
 *
 * Use this function in the vertical fill orientation case.
 */
static void
ewl_grid_vmap_position_next(Ewl_Grid *g, int *c, int *r)
{
	int col, row;
	
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_PARAM_PTR("c", c);
	DCHECK_PARAM_PTR("r", r);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);
	
	col = *c;
	row = *r;
	
	do {
		row++;
		if (row >= g->rows) {
			row = 0;
			col++;
		}
	} while (g->map[col + (row * g->cols)] != 0);

	*c = col;
	*r = row;
	
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param g: The grid to work with
 * @param c: a pointer to get the first free column position
 * @param r: a pointer to get the first free row position
 * @return Returns no value
 * @brief get the first free position for floating children
 */
static void
ewl_grid_map_start_position_get(Ewl_Grid *g, int *c, int *r)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("g", g);
	DCHECK_PARAM_PTR("c", c);
	DCHECK_PARAM_PTR("r", r);
	DCHECK_TYPE("g", g, EWL_GRID_TYPE);

	*c = 0;
	*r = 0;
	
	if (*g->map != 0) {
		if (g->orientation == EWL_ORIENTATION_HORIZONTAL)
			ewl_grid_hmap_position_next(g, c, r);
		else
			ewl_grid_vmap_position_next(g, c, r);
	}
	
	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param p: The container to work with
 * @param c: The widget to work with
 * @return Returns no value
 * @brief Notify the grid that a child has been added.
 */
void 
ewl_grid_child_add_cb(Ewl_Container *p, Ewl_Widget *c)
{
	Ewl_Grid *g;
	
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("p", p);
	DCHECK_PARAM_PTR("c", c);
	DCHECK_TYPE("p", p, EWL_CONTAINER_TYPE);
	DCHECK_TYPE("c", c, EWL_WIDGET_TYPE);

	g = EWL_GRID(p);
	g->data_dirty = TRUE;
	g->space++;

	if (g->space > (g->cols * g->rows)) {
		if (g->orientation == EWL_ORIENTATION_HORIZONTAL)
			ewl_grid_dimensions_set(g, g->cols, 
						(g->space / g->cols) + 1);
		else
			ewl_grid_dimensions_set(g, (g->space / g->rows) + 1, 
								g->rows);
	}

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param c: The Container
 * @param p: The chil to be deleted
 * @param idx: the index of the removed widget
 * @return Returns no value
 * @brief The child del callback
 */
void
ewl_grid_child_remove_cb(Ewl_Container *c, Ewl_Widget *w, int idx __UNUSED__)
{
	Ewl_Grid_Child *child;
	Ewl_Grid *g;

	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("c", c);
	DCHECK_TYPE("c", c, EWL_CONTAINER_TYPE);
	DCHECK_PARAM_PTR("w", w);
	DCHECK_TYPE("w", w, EWL_WIDGET_TYPE);

	g = EWL_GRID(c);
	child = (Ewl_Grid_Child *)ewl_widget_data_get(w, g);
	
	if (child) {
		g->space -= (child->end_row - child->start_row + 1)
			* (child->end_col - child->start_col + 1);
		FREE(child);
	}
	else 
		g->space--;
	
	IF_FREE(g->map);
	g->map = NULL;
	g->data_dirty = TRUE;

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

/**
 * @internal
 * @param p: The container to work with
 * @param child: The widget to work with
 * @param size: The new child size
 * @param o: The orientation
 * @return Returns no value
 * @brief Catch notification of child resizes.
 */
void
ewl_grid_child_resize_cb(Ewl_Container *p, Ewl_Widget *child, int size,
		        Ewl_Orientation o)
{
	DENTER_FUNCTION(DLEVEL_STABLE);
	DCHECK_PARAM_PTR("p", p);
	DCHECK_PARAM_PTR("child", child);
	DCHECK_TYPE("p", p, EWL_CONTAINER_TYPE);
	DCHECK_TYPE("child", child, EWL_WIDGET_TYPE);

	EWL_GRID(p)->data_dirty = TRUE;
	ewl_widget_configure(EWL_WIDGET(p));

	DLEAVE_FUNCTION(DLEVEL_STABLE);
}

