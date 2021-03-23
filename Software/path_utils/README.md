# json_path_to_csv.sh

This utility will converts a JSON-formatted trajectory created by PathWeaver into a CSV file containing all of the equivalent data.

## What does it do?

The PathWeaver JSON file contains a series of records like this
(pretty printed for clarity; PathWeaver tends to put this, and all subsequent records, all on one single line - at least on Linux):

```
{
  "time":0.0,
  "velocity":0.0,
  "acceleration":0.5,
  "pose": {
    "translation": {
      "x":0.3436362083556549,
      "y":1.933767601715334
    },
    "rotation": {
      "radians":-0.010988568682730925
    }
  },
  "curvature":0.0
}
```

This utility would convert that record to this (1 line per record in the final output):

```
0.0,0.0,0.5,0.3436362083556549,1.933767601715334,-0.010988568682730925,0.0
```
## Now what?

OK, I get it but what's the point?

The point is that you now have a CSV-formatted file (Command Separated Values);
something that can be easily imported into a spreadsheet.

Once you're in a spreadsheet, you can analyze this data in a whole bunch of different ways.
One of the most interesting ways I've used is to create a graph representing the overall path specified by
that trajectory.

Something like this:

![Path graph](https://cdn.discordapp.com/attachments/411354025762619406/823400704861143040/unknown.png)

### Cool - How do I do that?

You can get the path by creating a chart.

Specifically, create a "Scatter Graph" which permits you to specify X and Y coordinates.
For the X coordinate use the "D" column and for the Y coordinate, use the "E" column.

You can usually specify "Line only"
(which doesn't show the way points but you might want to see them in some cases;
PathWeaver tends to generate so many way points that they end up sort of being noise)
and you can also specify
to "smooth" the lines rather than do a jaggy point-to-point connection (this roughly emulates what trajectory
followers like ramsete will do anyway).

If you want an accurate representation of the path, you'll almost certainly want to adjust the overall dimensions and aspect
ratio of the chart that is produced in order to scale the X and Y axis similarly.

The resulting graph will give you an "Eye of God" sort of view - from above looking straight down.
Positive X values go up and positive Y values go right.

## What are the options?

There are several options which can be specified:

### -h

Include a single line of headers as the first line of the CSV file.
This will help you identify the columns easier after importing them into your spreadsheet.

Be sure to start one row lower when you create your scatter graph to skip the headers.

### -n

This will normalize all the generated output based on the first pose on the trajectory file.

Note that this basically does the same as the odometery reset using the initial pose that is typically done in
your code.  It can be useful to visualize a trajectory based on the starting point giving you insight
into the trajectory relative to your starting point.

## Show me an example!

json_path_to_csv reads from a trajectory file and writes its output to standard output.

Here is an example:

$ json_path_to_csv.sh -hn slalom.wpilib.json >slalom.json

## What are the dependencies?

Aside from a bash-compatible shell,
you'll need to have 'perl' (for the floating point math needed as part of the normalization calculations)
and 'jq' for the shell-level JSON processing.

## What about Windows?

Right now, json_path_to_csv is only for running on Linux.
If you wanted to run it under Windows, I'd suggest something like 'cygwin' to get a good bash-style shell.
It will get you 'perl' for normalization but you'll need to build and install 'jq' to get it to work
(pro hint:  'jq' is needed way more than 'perl' to do useful things).

If you get a Windows version running, consider contributing it back because it would be useful.






