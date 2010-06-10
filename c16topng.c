#include <riley/riley.h>
#include <riley/cairo.h>

#include <glib.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static gchar *output = NULL;

static GOptionEntry entries[] =
{
	{ "output", 'o', 0, G_OPTION_ARG_STRING, &output, "Use <prefix> to generate output filenames.", "<prefix>" },
	{ NULL }
};

int
main (int argc, char **argv)
{
	GError *error = NULL;
	GOptionContext *context;
	
	context = g_option_context_new ("file");
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_set_summary (context,
	  "Converts C16, S16, and BLK files into PNG files");
	g_option_context_set_description (context,
	  "Libriley version 0.1 under MIT License\n"
	  "Copyright (C) 2010 Michael Maltese <mchtly@gmail.com>\n");
	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_printerr ("%s\n", error->message);
		gchar *help = g_option_context_get_help (context, TRUE, NULL);
		g_print ("%s\n", help);
		g_free (help);
		g_free (context);
		exit (1);
	}
	g_free (context);
	
	if (2 != argc) {
	  g_printerr ("One (and only one) input file must be specified.\n");
		gchar *help = g_option_context_get_help (context, TRUE, NULL);
		g_print ("%s\n", help);
		g_free (help);
	  exit (1);
	}
	char *input = argv[1];
	
	if (!g_file_test (input, G_FILE_TEST_EXISTS)) {
	  g_printerr ("%s does not exist!\n", input);
	  exit (1);
	}
	if (!g_file_test (input, G_FILE_TEST_IS_REGULAR)) {
	  g_printerr ("%s must be a regular file (not a directory)\n", input);
	  exit (1);
	}
	
	if (!g_str_has_suffix(input, ".c16")
	 && !g_str_has_suffix(input, ".s16")
	 && !g_str_has_suffix(input, ".blk"))
  {
    g_printerr ("Cannot guess filetype of %s; make the filename ends in .c16,.s16, or .blk\n", input);
    exit(1);
  }
	
	FILE *fp = fopen (input, "r");
	if (!fp) {
	  g_printerr ("Could not open %s\n", input);
	  exit (1);
	}
	
	if (!output) {
	  output = g_strndup (input, strlen (input) - 4); // Remove extension
	}
	
	if (g_str_has_suffix (input, ".blk"))
	{
	  blk_t *blk = blk_new_from_file (fp);
	  fclose (fp);
	  if (!blk) {
	    g_printerr ("Could not load %s; bug or malformed BLK file?\n", input);
	    exit (1);
	  }
	 
	  char name[strlen(output) + 4];
	  sprintf ((char*)name, "%s.png", output);
	  
	  cairo_surface_t *surface = blk_get_cairo (blk);
	  cairo_status_t status = cairo_surface_write_to_png (surface, name);
	  cairo_surface_destroy (surface);
	  blk_destroy (blk);
	  
	  if (CAIRO_STATUS_SUCCESS != status) {
	    g_printerr ("Failed writing %s\n", name);
	    exit (1);
	  }
	}
	else if (g_str_has_suffix (input, ".c16")
	      || g_str_has_suffix (input, ".s16"))
	{
	  c16_t *c16 = NULL;
	  if (g_str_has_suffix (input, ".c16")) c16 = c16_new_from_file (fp);
	  else c16 = s16_new_from_file (fp);
	  fclose (fp);
	  if (!c16) {
	    g_printerr ("Could not load %s; bug or malformed C16/S16 file?\n", input);
	    exit (1);
	  }
	  
	  for (int i = 0; i < c16_get_number_of_sprites(c16); ++i)
	  {
	    char *name = g_strdup_printf("%s_%i.png", output, i);
	    if (!name) {
	      g_printerr ("g_strdup_printf failed; aborting!\n");
	      abort();
	    }
	    
	    cairo_surface_t *surface = c16_get_sprite_cairo (c16, i);
	    cairo_status_t status = cairo_surface_write_to_png (surface, name);
	    cairo_surface_destroy (surface);
	    
	    if (CAIRO_STATUS_SUCCESS != status) {
	      g_printerr ("Failed writing %s\n", name);
	      free (name);
	      c16_destroy (c16);
	      exit (1);
	    }
	    
	    free (name);
	  }
	  
	  c16_destroy (c16);
	}
	else assert (0);
	
}