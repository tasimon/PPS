
/*----------------------------------------------------------------------------
 *                                  E.S.O.
 *----------------------------------------------------------------------------
 * File name    :   gnuplot_pipes.c
 * Author       :   N. Devillard
 * Created on   :   Fri Sept 26 1997
 * Language     :   ANSI C
 * Description  :   C interface to gnuplot
 *
 *  gnuplot is a freely available, command-driven graphical display tool for
 *  Unix. It compiles and works quite well on a number of Unix flavours as
 *  well as other operating systems. The following module enables sending
 *  display requests to gnuplot through simple C calls.
 *
 *--------------------------------------------------------------------------*/

/*

 $Id: gnuplot_pipes.c,v 1.5 1999/04/27 11:11:20 ndevilla Exp $
 $Author: ndevilla $
 $Date: 1999/04/27 11:11:20 $
 $Revision: 1.5 $
 $Log: gnuplot_pipes.c,v $
 Revision 1.5  1999/04/27 11:11:20  ndevilla
 indented

 Revision 1.4  1999/04/27 11:10:45  ndevilla
 added printf-like interface

 Revision 1.3  1999/02/22 17:44:09  ndevilla
 corrected static string modification to allow gcc to compile Ok.

 Revision 1.2  1998/06/12 15:23:01  ndevilla
 changed API

 Revision 1.2  1998/04/29 07:59:31  ndevilla
 slight modifs in plotting style

 Revision 1.1  1998/03/24 14:04:14  ndevilla
 Initial revision

 */

/*----------------------------------------------------------------------------
 *                              Includes
 *--------------------------------------------------------------------------*/

#include "gnuplot_pipes.h"

/*----------------------------------------------------------------------------
 *                              Defines
 *--------------------------------------------------------------------------*/

#define GP_CMD_SIZE     1024
#define GP_TITLE_SIZE   80
#define GP_EQ_SIZE      512
#define GP_LINE_SIZE    80



/*----------------------------------------------------------------------------
 *                          Function codes
 *--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
   Function :   check_X_display()
   In       :   void
   Out      :   int 0 if display Ok, 1 if not
   Job      :   checks out the DISPLAY environment variable to see if it
                is set or not.
   Notice   :
 ---------------------------------------------------------------------------*/

int check_X_display(void)
{
    char    *   display ;
    
    display = getenv("DISPLAY") ;
    if (display == NULL) {
        fprintf(stderr, "cannot find DISPLAY variable: is it set?\n") ;
        return 1 ;
    } else {
        return 0 ;
    }
}




/*----------------------------------------------------------------------------
 * Function :   gnuplot_init()
 * In       :   void
 * Out      :   gnuplot control handle
 * Job      :   opens up a gnuplot session ready to receive commands
 * Notice   :
 *--------------------------------------------------------------------------*/

gnuplot_ctrl * gnuplot_init(void)
{
    gnuplot_ctrl *  handle ;

    if (check_X_display()) return NULL ;

    /* 
     * Structure initialization:
     */
    handle = malloc(sizeof(gnuplot_ctrl)) ;
    handle->nplots = 0 ;
    gnuplot_setstyle(handle, "points") ;
    handle->ntmp = 0 ;

    handle->gnucmd = popen("gnuplot", "w") ;
    if (handle->gnucmd == NULL) {
        fprintf(stderr, "cannot find gnuplot program: check $path\n") ;
        free(handle) ;
        return NULL ;
    }
    return handle;
}

/*----------------------------------------------------------------------------
 * Function :   gnuplot_close()
 * In       :   gnuplot control handle
 * Out      :   void
 * Job      :   close a gnuplot session previously opened by gnuplot_init()
 * Notice   :   kills the child PID.
 *              deletes all opened temporary files
 *--------------------------------------------------------------------------*/

void gnuplot_close(gnuplot_ctrl * handle)
{
    int     i ;
    if (check_X_display()) return ;
    if (handle->ntmp) {
        for (i=0 ; i<handle->ntmp ; i++) {
            remove(handle->to_delete[i]) ;
        }
    }
    if (pclose(handle->gnucmd) == -1) {
        fprintf(stderr, "problem closing communication to gnuplot\n") ;
        return ;
    }
    free(handle) ;
    return ;
}

/*----------------------------------------------------------------------------
 * Function :   gnuplot_cmd()
 * In       :   gnuplot control handle, character string a la printf
 * Out      :   void
 * Job      :   sends a character string to gnuplot, as if typed by user in
 *              an interactive session.
 * Notice   :   no control is done on the ouput of the command.
 *--------------------------------------------------------------------------*/

void gnuplot_cmd(
    gnuplot_ctrl *  handle,
    char         *  cmd, 
    ...)
{
    va_list ap ;
    char    local_cmd[GP_CMD_SIZE];

    va_start(ap, cmd);
    vsprintf(local_cmd, cmd, ap);
    va_end(ap);

    strcat(local_cmd, "\n");

    fputs(local_cmd, handle->gnucmd) ;
    fflush(handle->gnucmd) ;
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_setstyle()
 * In       :   plotting style: a character string taken from the
 *              following set:
 *              "lines"
 *              "points"
 *              "linespoints"
 *              "impulses"
 *              "dots"
 *              "steps"
 *              "errorbars"
 *              "boxes"
 *              "boxeserrorbars"
 * Out      :   void
 * Job      :   set a plotting style for a given gnuplot session
 * Notice   :
 *--------------------------------------------------------------------------*/

void gnuplot_setstyle(gnuplot_ctrl * h, char * plot_style) 
{
    if (strcmp(plot_style, "lines") &&
        strcmp(plot_style, "points") &&
        strcmp(plot_style, "linespoints") &&
        strcmp(plot_style, "impulses") &&
        strcmp(plot_style, "dots") &&
        strcmp(plot_style, "steps") &&
        strcmp(plot_style, "errorbars") &&
        strcmp(plot_style, "boxes") &&
        strcmp(plot_style, "boxerrorbars")) {
        fprintf(stderr, "warning: unknown requested style: using points\n") ;
        (void)strcpy(h->pstyle, "points") ;
    } else {
        (void)strcpy(h->pstyle, plot_style) ;
    }
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_set_xlabel()
 * In       :   handle to gnuplot_ctrl, x label
 * Out      :   void
 * Job      :   sets the x label to be printed out with this handle
 * Notice   :
 *--------------------------------------------------------------------------*/
void gnuplot_set_xlabel(gnuplot_ctrl * h, char * label)
{
    char    cmd[GP_CMD_SIZE] ;

    (void)sprintf(cmd, "set xlabel \"%s\"", label) ;
    gnuplot_cmd(h, cmd) ;
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_set_ylabel()
 * In       :   handle to gnuplot_ctrl, y label
 * Out      :   void
 * Job      :   sets the y label to be printed out with this handle
 * Notice   :
 *--------------------------------------------------------------------------*/
void gnuplot_set_ylabel(gnuplot_ctrl * h, char * label)
{
    char    cmd[GP_CMD_SIZE] ;

    (void)sprintf(cmd, "set ylabel \"%s\"", label) ;
    gnuplot_cmd(h, cmd) ;
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_resetplot()
 * In       :   handle to gnuplot_ctrl
 * Out      :   void
 * Job      :   reset a plot (next plot with erase previous ones)
 * Notice   :
 *--------------------------------------------------------------------------*/

void gnuplot_resetplot(gnuplot_ctrl * h)
{
    int     i ;
    if (h->ntmp) {
        for (i=0 ; i<h->ntmp ; i++) {
            remove(h->to_delete[i]) ;
        }
    }
    h->ntmp = 0 ;
    h->nplots = 0 ;
    return ;
}

/*----------------------------------------------------------------------------
 * Function :   gnuplot_plot1d_var1()
 * In       :   handle to gnuplot_ctrl, list of doubles, number of
 *              doubles in this list, title for the output graph.
 * Out      :   void
 * Job      :   plots out a 2d graph from a list of doubles. The
 *              x-coordinate is the double index, the y-coordinate is
 *              the double itself.
 * Notice   :
 * Example  :
 
    gnuplot_ctrl    *h ;
    double          d[50] ;
    int             i ;

    h = gnuplot_init() ;
    for (i=0 ; i<50 ; i++) {
        d[i] = (double)(i*i) ;
    }
    gnuplot_plot1d_var1(h, d, 50, "parabola") ;
    sleep(2) ;
    gnuplot_close(h) ;

 *--------------------------------------------------------------------------*/

void gnuplot_plot1d_var1(
    gnuplot_ctrl    *   handle,
    double          *   d,
    int                 n_point,
    char            *   title
)
{
    int         i ;
    FILE    *   tmp ;
    char    *   name ;
    char        cmd[GP_CMD_SIZE] ;
    char        line[GP_LINE_SIZE] ;

    /* can we open one more temporary file? */
    if (handle->ntmp == GP_MAX_TMP_FILES - 1) {
        fprintf(stderr,
                "maximum # of temporary files reached (%d): cannot open more",
                GP_MAX_TMP_FILES) ;
        return ;
    }

    /* Open temporary file for output   */
    if ((name = tmpnam(NULL)) == (char*)NULL) {
        fprintf(stderr,"cannot create temporary file: exiting plot") ;
        return ;
    }
    if ((tmp = fopen(name, "w")) == NULL) {
        fprintf(stderr, "cannot create temporary file: exiting plot") ;
        return ;
    }

    /* Store file name in array for future deletion */
    (void)strcpy(handle->to_delete[handle->ntmp], name) ;
    handle->ntmp ++ ;

    /* Write data to this file  */
    for (i=0 ; i<n_point ; i++) {
        (void)fprintf(tmp, "%f\n", d[i]) ;
    }
    (void)fflush(tmp) ;
    (void)fclose(tmp) ;

    /* Command to be sent to gnuplot    */
    if (handle->nplots > 0) {
        (void)strcpy(cmd, "replot") ;
    } else {
        (void)strcpy(cmd, "plot") ;
    }
    
    if (title == NULL) {
        (void)sprintf(line, "%s \"%s\" with %s", cmd, name, handle->pstyle) ;
    } else {
        (void)sprintf(line, "%s \"%s\" title \"%s\" with %s", cmd, name,
                      title, handle->pstyle) ;
    }

    /* send command to gnuplot  */
    gnuplot_cmd(handle, line) ;
    handle->nplots++ ;
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_plot1d_var2()
 * In       :   handle to gnuplot_ctrl, list of dpoints, number of
 *              points in this list, title for the output graph.
 * Out      :   void
 * Job      :   plots out a 2d graph from a list of dpoints.
 * Notice   :
 * Example  :
 
    gnuplot_ctrl    *h ;
    dpoint          d[50] ;
    int             i ;

    h = gnuplot_init() ;
    for (i=0 ; i<50 ; i++) {
        d[i].x = (double)(i)/10.0 ;
        d[i].y = d[i].x * d[i].x ;
    }
    gnuplot_plot1d_var2(h, d, 50, "parabola") ;
    sleep(2) ;
    gnuplot_close(h) ;

 *--------------------------------------------------------------------------*/


void gnuplot_plot1d_var2(
    gnuplot_ctrl    *   handle,
    dpoint          *   d,
    int                 n_points,
    char            *   title
)
{
    int         i ;
    FILE    *   tmp ;
    char    *   name ;
    char        cmd[GP_CMD_SIZE] ;
    char        line[GP_LINE_SIZE] ;

    /* can we open one more temporary file? */
    if (handle->ntmp == GP_MAX_TMP_FILES - 1) {
        fprintf(stderr,
                "maximum # of temporary files reached (%d): cannot open more",
                GP_MAX_TMP_FILES) ;
        return ;
    }

    /* Open temporary file for output   */
    if ((name = tmpnam(NULL)) == (char*)NULL) {
        fprintf(stderr,"cannot create temporary file: exiting plot") ;
        return ;
    }
    if ((tmp = fopen(name, "w")) == NULL) {
        fprintf(stderr,"cannot create temporary file: exiting plot") ;
        return ;
    }

    /* Store file name in array for future deletion */
    (void)strcpy(handle->to_delete[handle->ntmp], name) ;
    handle->ntmp ++ ;

    /* Write data to this file  */
    for (i=0 ; i<n_points ; i++) {
        (void)fprintf(tmp, "%f %f\n", d[i].x, d[i].y) ;
    }
    (void)fflush(tmp) ;
    (void)fclose(tmp) ;

    /* Command to be sent to gnuplot    */
    if (handle->nplots > 0) {
        (void)strcpy(cmd, "replot") ;
    } else {
        (void)strcpy(cmd, "plot") ;
    }
    
    if (title == NULL) {
        (void)sprintf(line, "%s \"%s\" with %s", cmd, name, handle->pstyle) ;
    } else {
        (void)sprintf(line, "%s \"%s\" title \"%s\" with %s", cmd, name,
                      title, handle->pstyle) ;
    }

    /* send command to gnuplot  */
    gnuplot_cmd(handle, line) ;
    handle->nplots++ ;
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_plot_slope()
 * In       :   handle to gnuplot_ctrl, slope, y-intercept, graph title.
 * Out      :   void
 * Job      :   Plot out a slope on a gnuplot session
 * Notice   :   The provided slope has an equation of the form:
 *              y = a * x + b
 *              a and b are doubles.
 * Example  :

    gnuplot_ctrl    *   h ;
    double              a, b ;

    h = gnuplot_init() ;
    gnuplot_plot_slope(h, 1.0, 0.0, "unity slope") ;
    sleep(2) ;
    gnuplot_close(h) ;
    
 *--------------------------------------------------------------------------*/


void gnuplot_plot_slope(
    gnuplot_ctrl    *   handle,
    double              a,
    double              b,
    char            *   title
)
{
    char    stitle[GP_TITLE_SIZE] ;
    char    cmd[GP_CMD_SIZE] ;

    if (title == NULL) {
        (void)strcpy(stitle, "no title") ;
    } else {
        (void)strcpy(stitle, title) ;
    }

    if (handle->nplots > 0) {
        (void)sprintf(cmd, "replot %f * x + %f title \"%s\" with %s",
                      a, b, title, handle->pstyle) ;
    } else {
        (void)sprintf(cmd, "plot %f * x + %f title \"%s\" with %s",
                      a, b, title, handle->pstyle) ;
    }
    gnuplot_cmd(handle, cmd) ;
    handle->nplots++ ;
    return ;
}


/*----------------------------------------------------------------------------
 * Function :   gnuplot_plot_equation()
 * In       :   handle to gnuplot_ctrl, character string containing the
 *              equation to plot, graph title.
 * Out      :   void
 * Job      :   Plots out a curve of given equation. The general form of
 *              the equation is y = f(x)
 * Notice   :
 * Example  :

        gnuplot_ctrl    *h ;
        char            eq[80] ;

        h = gnuplot_init() ;
        strcpy(eq, "sin(x) * cos(2*x)") ;
        gnuplot_plot_equation(h, eq, "sine wave", normal) ;
        gnuplot_close(h) ;

 *--------------------------------------------------------------------------*/

void gnuplot_plot_equation(
    gnuplot_ctrl    *   h,
    char            *   equation,
    char            *   title
)
{
    char    cmd[GP_CMD_SIZE];
    char    plot_str[GP_EQ_SIZE] ;
    char    title_str[GP_TITLE_SIZE] ;

    if (title == NULL) {
        (void)strcpy(title_str, "no title") ;
    } else {
        (void)strcpy(title_str, title) ;
    }
    if (h->nplots > 0) {
        (void)strcpy(plot_str, "replot") ;
    } else {
        (void)strcpy(plot_str, "plot") ;
    }

    (void)sprintf(cmd, "%s %s title \"%s\" with %s", 
                  plot_str, equation, title_str, h->pstyle) ;
    gnuplot_cmd(h, cmd) ;
    h->nplots++ ;
    return ;
}


/*----------------------------- end of file --------------------------------*/
