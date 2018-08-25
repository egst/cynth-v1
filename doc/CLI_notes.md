# CLI

for full interactive setup:

```cli
// Prints avilable devices and guides the user through their setup:
cynth setup
```

for manual setup:

```cli
// Prints available devices:
cynth get cynth::renderingDevices

// Sets variables:
// Syntax: [namespace::]var=val
cynth set cynth::channels=2
cynth set cynth::renderingDevices[0]=0
cynth set cynth::renderingDevices[1]=1
```

synth from file:

```cli
// Executes cynth script:
cynth run file.cynth
```

manual synth:

```cli
// Creates new device:
// Params: type name wf freq amp
cynth new toneGenerator foo_tone cynth::sine 440 100
cynth new LFO foo_lfo cynth::square 0.5 50
// (LFOs use different relative frequency.)

// Connects two devices:
// Params: controller controlled property
cynth connect foo_lfo foo_tone amp
cynth connect foo_tone cynth::out stream

// Set Meassures per Minute:
cynth set cynth::mpm=30

// Start the devices:
cynth start foo_tone
cynth start foo_lfo
```

rack:

```cli
// Prints the current rack view:
cynth rack

// Output example:
connections:
foo_lfo -> foo_tone.amp
foo_tone -> cynth::out

running:
[name]      [wf]            [freq]  [amp]
foo_lfo     cynth::square   0.5     50
foo_tone    cynth::sine     440     100
```

cynth script file:

```cli
toneGenerator foo_tone {
    wf: cynth::sine,
    freq: 440,
    amp: 100
};
LFO foo_lfo {
    wf: cynth::square,
    freq: 0.5,
    amp: 50
};
foo_lfo -> foo_tone.amp;
foo_tone -> cynth::out.stream;
set cynth::mpm = 30;
start foo_lfo;
start foo_tone;
```