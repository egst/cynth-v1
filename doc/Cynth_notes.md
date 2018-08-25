# Cynth

## Parsing CLI commands

```chart
Commands:                          Script
get set new connect turn rack run  |
\___________________________/  |   |
                          |    +---+
                          |    |
                        / |   Cynth::Parser::separateCommands()
                        | |    |
Cynth::CLI              | +----+
     ::processCommand()-|   |
                        |  (string)
                        |   |
                        |  Cynth::Parser::identifyCommand() -> type
                        |
                        |  Cynth::Parser::parseCommand<type>() -> Cynth::Command
                        |
                        \  Cynth::Command()
```

# User Interface

## Rack

Rack holds the required devices, connects them, and controls their activity.
A rack may hold connected input devices such as MIDI keyboard and oscillator
devices such as tone generator or LFO. Every rack provides output to the
selected physical device - speakers or a sound card. Also every rack has a
metronome device which every oscillator device in the rack may use to oscillate
with a relative frequency.

Cynth provices a custom command line interface for interaction with the rack.
Once you start the program, you will se a prompt to enter your commands:

```cynth
~
```

To create a new rack:

```cynth
~ rack new [rack_name]
```

To select this rack:

```cynth
~ rack select [rack_name]
```

To view the current setup of a rack:

```cynth
~ rack view [rack_name]
```

To view the setup of the current rack:

```cynth
~ rack view
```

To list currently open racks:

```cynth
~ rack list
```

Racks are an equivalent of tabs in other programs. They are only meant as a
workspace to hold your projects. There is no option to save a rack. To save your
project, use cynth script (described below).

## Commands

Once you select a rack, using commands `set`, `get`, `new`, `connect` and `turn`
will only affect this rack.

To set a variable:

```cynth
~ set [var_name] [value]
```

Available variables are:

`tempo_type`: units of tempo

Available values: `mpm` (measures per minute), `bpm` (beats per minute)

`tempo`: tempo in the selected units

Values: integral

`channels`: number of output channels

`output_[n]`: output device for the `n`-th channel

Value: integral values from `info_output_devices` (described below)

Some variables are read only:

`info_output_devices`: list of available output devices

`info_[device_type]`: information about a device type (described below)

To print value of a variable:

```cynth
~ get [var_name]
```

To create a new device:

```cynth
~ new [device_type] [device_name] [device_properties]...
```

Use `get info_[device_type]` to get a list of the properties.

To connect the existing devices:

```cynth
~ connect [device1_name] [device2_name] [device2_property]
```

The output of the first device then modulates the selected property of the
second one.

To connect the output of a device to the output device (to produce sound), use:

```cynth
~ connect [device_name] _output_ stream
```

To turn the existing devices on/off:

```cynth
~ turn [device_name] on
~ turn [device_name] off
````

To run a cynth script:

```cynth
~ run [cynth_script_path]
```

## Cynth script

Interaction through CLI directly is good for testing, but using cynth script
may be more convenient for larger projects.

Cynth script is a file with `.cynth` extension which holds the commands to
execute. Every command described above may be used in the cynth script, except
the `get` command, as it is usefull only for testing purposes, and the `rack`
command, as the cynth script only modifies the current rack.

For better readability, cynth script uses a bit more complex syntax:

`set`:

```cynth
[var_name] = [value];
```

`new`:

```cynth
[device_type] [device_name] {
    [property_name]: [value],
    ...
};
```

`connect`:

```cynth
[device1_name] -> [device2_name].[device2_property];
```

`turn`:

```cynth
[device_name] on;
[device_name] off;
```

`run`:

```cynth
run [cynth_script_path];
```

The `run` command inside a cynth script may be used to separate your project
into modules.

Spaces, linebrakes, and indentation may be customized to your style. Commands
are separated only by the semicolons, not linebreaks. Only the spaces around
variable names and keywords are mandatory.

The complete workflow process then may look like this:

```cynth
~ rack new foo_rack
```

Then create a cynth script `foo_synth.cynth`:

```cynth
toneGenerator foo_tone {
    wf: ..., // TODO
    freq: 440,
    amp: 0 // Silent by default, controlled by a midi controller.
};

lowPassFilter foo_filter {
    freq: 500,
    gain: 50,
    q: 2 // TODO: range of Q
};

envelope foo_env {
    attack: 100,
    decay: 50,
    sustain: 500,
    release: 200
};

foo_env -> foo_tone.amp;
foo_env -> foo_filter.gain;
foo_tone -> foo_filter.stream;
foo_filter -> _output_.stream;

run baz_keyboard.cynth;

baz_keyboard -> foo_env.trigger;

foo_env on;
foo_filter on;
foo_tone on;
```

It will run another cynth script `baz_keyboard.cynth`:

```cynth
midiController baz_keyboard {
    device: ..., // TODO
    key_1: 261.63,
    key_2: 293.67,
    ...
};
```

Then just run it in the CLI:

```cynth
~ run foo_synth.cynth
```