
/******************************************************************************
* MODULE     : edit_math.cpp
* DESCRIPTION: modify mathematical structures
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/

#include "edit_math.hpp"

/******************************************************************************
* Constructors and destructors
******************************************************************************/

edit_math_rep::edit_math_rep () {}
edit_math_rep::~edit_math_rep () {}

/******************************************************************************
* Making mathematical objects
******************************************************************************/

void
edit_math_rep::make_group () {
  if (selection_active_small ())
    insert_tree (tree (GROUP, selection_get_cut ()));
  else {
    insert_tree (tree (GROUP, ""), path (0, 0));
    set_message ("move to the right when finished", "group");
  }
}

void
edit_math_rep::make_lprime (string s) {
  tree& st= subtree (et, path_up (tp));
  if (is_func (st, LPRIME, 1) && (last_item (tp) == 1)) {
    if (is_atomic (st[0]))
      insert (path_up (tp) * path (0, N (st[0]->label)), s);
  }
  else insert_tree (tree (LPRIME, s));
}

void
edit_math_rep::make_rprime (string s) {
  tree& st= subtree (et, path_up (tp));
  if (is_func (st, RPRIME, 1) && (last_item (tp) == 1)) {
    if (is_atomic (st[0]))
      insert (path_up (tp) * path (0, N (st[0]->label)), s);
  }
  else insert_tree (tree (RPRIME, s));
}

void
edit_math_rep::make_below () {
  if (selection_active_small ()) {
    insert_tree (tree (BELOW, selection_get_cut (), ""), path (1, 0));
    set_message ("type script, move right", "under");
  }
  else {
    insert_tree (tree (BELOW, "", ""), path (0, 0));
    set_message ("type body, move down, type script", "under");
  }
}

void
edit_math_rep::make_above () {
  if (selection_active_small ()) {
    insert_tree (tree (ABOVE, selection_get_cut (), ""), path (1, 0));
    set_message ("type script, move right", "above");
  }
  else {
    insert_tree (tree (ABOVE, "", ""), path (0, 0));
    set_message ("type body, move up, type script", "above");
  }
}

void
edit_math_rep::make_script (bool sup, bool right) {
  tree_label s (sup? SUP (right): SUB (right));
  if (selection_active_small ())
    insert_tree (tree (s, selection_get_cut ()));
  else {
    path   p= path_up (tp);
    tree   t= subtree (et, p);
    bool   flag;

    if (is_format (p))
      fatal_error ("bad cursor position", "edit_math_rep::make_script");
    if (is_script (t, flag) && (flag==right) && (L(t)==s)) {
      go_to_end (p * 0);
      return;
    }
    insert_tree (tree (s, ""), path (0, 0));
    set_message ("move to the right when finished",
		 (char*) (sup? (right? "superscript": "left superscript"):
			       (right? "subscript": "left subscript")));
  }
}

void
edit_math_rep::make_fraction () {
  if (selection_active_small ()) {
    insert_tree (tree (FRAC, selection_get_cut (), ""), path (1, 0));
    set_message ("type denominator, move right", "fraction");
  }
  else {
    insert_tree (tree (FRAC, "", ""), path (0, 0));
    set_message ("type numerator, move down, type denominator", "fraction");
  }
}

void
edit_math_rep::make_sqrt () {
  if (selection_active_small ())
    insert_tree (tree (SQRT, selection_get_cut ()));
  else {
    insert_tree (tree (SQRT, ""), path (0, 0));
    set_message ("move to the right when finished", "square root");
  }
}

void
edit_math_rep::make_var_sqrt () {
  if (selection_active_small ()) {
    tree t= selection_get_cut ();
    if (is_func (t, SQRT, 1))
      insert_tree (tree (SQRT, t[0], ""), path (1, 0));
    else insert_tree (tree (SQRT, t, ""), path (1, 0));
  }
  else {
    insert_tree (tree (SQRT, "", ""), path (0, 0));
    set_message ("left: set n, right: when finished", "n-th root");
  }
}

void
edit_math_rep::make_wide (string wide) {
  if (selection_active_small ())
    insert_tree (tree (WIDE, selection_get_cut (), wide));
  else {
    insert_tree (tree (WIDE, "", wide), path (0, 0));
    set_message ("move to the right when finished", "wide accent");
  }
}

void
edit_math_rep::make_wide_under (string wide) {
  if (selection_active_small ())
    insert_tree (tree (VAR_WIDE, selection_get_cut (), wide));
  else {
    insert_tree (tree (VAR_WIDE, "", wide), path (0, 0));
    set_message ("move to the right when finished", "wide under accent");
  }
}

void
edit_math_rep::make_neg () {
  if (selection_active_small ())
    insert_tree (tree (NEG, selection_get_cut ()));
  else {
    insert_tree (tree (NEG, ""), path (0, 0));
    set_message ("move to the right when finished", "negation");
  }
}

/******************************************************************************
* Deleting mathematical objects
******************************************************************************/

void
edit_math_rep::back_prime (tree t, path p, bool forward) {
  if ((N(t) == 1) && is_atomic (t[0])) {
    string s= t[0]->label;
    if (forward) {
      int i= 0, n= N(s);
      if ((i<n) && (s[i] == '<'))
	for (; i<n; i++)
	  if (s[i] == '>') break;
      if (i>=n-1) {
	assign (p, "");
	correct (path_up (p));
      }
      else remove (p * path (0, 0), i+1);
    }
    else {
      int i= N(s)-1;
      if ((i>=0) && (s[i] == '>'))
	for (; i>=0; i--)
	  if (s[i] == '<') break;
      if (i<=0) {
	assign (p, "");
	correct (path_up (p));
      }
      else remove (p * path (0, i), N(s)-i);
    }
  }
}

void
edit_math_rep::back_in_wide (tree t, path p, bool forward) {
  int i= last_item (p);
  if ((i == 0) && is_empty (t[0])) {
    assign (path_up (p), "");
    correct (path_up (p, 2));
  }
  else go_to_border (path_up (p), !forward);
}

/******************************************************************************
* Trees
******************************************************************************/

void
edit_math_rep::make_tree () {
  if (selection_active_small ())
    insert_tree (tree (TREE, selection_get_cut (), ""), path (1, 0));
  else {
    insert_tree (tree (TREE, "", ""), path (0, 0));
    set_message ("E-right: insert a new branch", "tree");
  }
}

bool
edit_math_rep::inside_tree () {
  int i;
  return !nil (get_tree (i));
}

path
edit_math_rep::get_tree (int& i) {
  path p= tp;
  while (true) {
    path q= path_up (p);
    if (!(rp < q)) return path ();
    tree t= subtree (et, q);
    if (is_func (t, TABLE)) return path ();
    if (is_func (t, TREE)) {
      i= last_item (p);
      return q;
    }
    p= q;
  }
}

void
edit_math_rep::branch_insert (bool at_right) {
  int i;
  path p= get_tree (i);
  if (nil (p)) return;
  if (i==0) {
    if (at_right) i= N (subtree (et, p));
    else i= 1;
  }
  else if (at_right) i++;
  insert (p * i, tree (TREE, ""));
  go_to (p * path (i, 0));
}

void
edit_math_rep::branch_delete (bool forward) {
  int i;
  path p= get_tree (i);
  if (nil (p) || (i==0)) return;
  if (N (subtree (et, p)) == 2) {
    assign (p, subtree (et, p * 0));
    correct (path_up (p));
  }
  else {
    remove (p * i, 1);
    if (forward) {
      if (i == N (subtree (et, p))) go_to_end (p);
      else go_to_start (p * i);
    }
  }
}

void
edit_math_rep::back_in_tree (tree t, path p, bool forward) {
  int i= last_item (p);
  if (i>0) {
    if ((i>0) && (t[i] == "")) branch_delete (forward);
    else if (!forward) go_to_end (path_up (p) * (i-1));
    else if (i == N(t)-1) go_to_end (path_up (p));
    else go_to_start (path_up (p) * (i+1));
  }
  else {
    if (t == tree (TREE, "", "")) {
      p= path_up (p);
      assign (p, "");
      correct (path_up (p));
    }
    else if (forward) go_to_start (path_inc (p));
    else go_to_start (path_up (p));
  }
}
