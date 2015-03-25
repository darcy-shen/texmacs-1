
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : client-widgets.scm
;; DESCRIPTION : widgets for remote clients
;; COPYRIGHT   : (C) 2015  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (client client-widgets)
  (:use (client client-tmfs)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Permissions editor
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (set-permission-all perms server id attr on?)
  (with old-vals (ahash-ref perms attr)
    (with new-vals (append (if on? (list "all") (list))
                           (list-difference old-vals (list "all")))
      (ahash-set! perms attr new-vals)
      (remote-set-field server id attr new-vals))))

(define (set-permissions perms enc server id attr vals*)
  (with vals (map (cut ahash-ref enc <>) vals*)
    (ahash-set! perms attr vals)
    (remote-set-field server id attr vals)))

(define (get-permissions perms dec server id attr)
  (with vals (list-union (ahash-ref perms "owner")
                         (ahash-ref perms attr))
    (with vals* (map (cut ahash-ref dec <>) vals)
      (sort vals* string<=?))))

(tm-widget ((entry-permissions-editor server id attrs users enc dec perms) quit)
  (padded
    (tabs
      (loop (attr attrs)
        (tab (text (upcase-first attr))
          (padded
            (hlist
              (toggle (begin
                        (set-permission-all perms server id attr answer)
                        (refresh-now "permission-checklist"))
                      (in? "all" (get-permissions perms dec server id attr)))
              // //
              (text "All users") >>)
            ===
            (refreshable "permission-checklist"
              (if (nin? "all" (get-permissions perms dec server id attr))
                  (choices (set-permissions perms enc server id attr answer)
                           (map (cut ahash-ref dec <>) users)
                           (get-permissions perms dec server id attr)))
              (if (in? "all" (get-permissions perms dec server id attr))
                  (resize "250px" "100px"
                    (text ""))))))))))

(tm-define (open-entry-permissions-editor server id attrs)
  (:interactive #t)
  (with-remote-search-user users server (list)
    (let* ((perms (make-ahash-table))
           (enc (make-ahash-table))
           (dec (make-ahash-table)))
      (with-remote-get-entry entry server id
        (for (attr attrs)
          (with vals (or (assoc-ref entry attr) (list))
            (ahash-set! perms attr vals)
            (set! users (list-union users vals))))
        (with-remote-get-user-pseudo pseudos server users
          (with-remote-get-user-name names server users
            (ahash-set! dec "all" "all")
            (ahash-set! enc "all" "all")
            (for-each (lambda (user pseudo name)
                        (when (and (string? pseudo) (string? name))
                          (with full (string-append pseudo " (" name ")")
                            (ahash-set! dec user full)
                            (ahash-set! enc full user))))
                      users pseudos names)
            (set! users (list-difference users (list "all")))
            (dialogue-window (entry-permissions-editor server id attrs
                                                       users enc dec perms)
                             noop "Change permissions")))))))

(tm-define (open-file-permissions-editor server u)
  (:interactive #t)
  (with name (resource-cache-get (url->string u) "name")
    (with-remote-search ids server (list (cons "name" name))
      (when (pair? ids)
        (with attrs (list "readable" "writable" "owner")
          (open-entry-permissions-editor server (car ids) attrs))))))
