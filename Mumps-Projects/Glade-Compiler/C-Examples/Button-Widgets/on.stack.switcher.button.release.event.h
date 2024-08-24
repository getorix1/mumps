void on_stack_switcher_button_release_event(GtkWidget *w) {
 printf("%s\n", "on.stack.switcher.button.release.event.h");
 printf("\tButton  clicked\n");
 printf("\tVisible Child: %s\n",gtk_stack_get_visible_child_name(GTK_STACK(stack1)));
 }
