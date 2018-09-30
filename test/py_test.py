import math
from Cynth import *

wave_fs_library = WaveFs()

sound_card = SoundCard()

sound_card.printProperties()

lead = ToneGenerator(wave_f = wave_func_t.SAW)

bass = ToneGenerator(wave_f = wave_func_t.SAW)

lfo_vibrato = LFO(wave_f = wave_func_t.SINE_POS,
                  freq   = 10.0,
                  amp    = 0.5,
                  freq_t = freq_type_t.HZ)

lfo_wah     = LFO(wave_f = wave_func_t.SINE_POS,
                  freq   = 1.0,
                  amp    = 0.5,
                  freq_t = freq_type_t.HZ)

seq_arp_1 = SequenceFunction()
seq_arp_1 << SequenceElement(0.25,  0.4, 261.63)   # C        1/4 ^
seq_arp_1 << SequenceElement(0.125)                # Silence  1/8
seq_arp_1 << SequenceElement(0.125, 0.2, 329.63)   # E        1/8
seq_arp_1 << SequenceElement(0.375, 0.4, 392)      # G        3/8 ^
seq_arp_1 << SequenceElement(0.125, 0.2, 493.88)   # B        1/8
seq_arp_1 *= 2

seq_arp_2 = SequenceFunction()
seq_arp_2 << SequenceElement(0.25,  0.4, 233.08)   # A#       1/4 ^
seq_arp_2 << SequenceElement(0.125)                # Silence  1/8
seq_arp_2 << SequenceElement(0.125, 0.2, 329.63)   # E        1/8
seq_arp_2 << SequenceElement(0.375, 0.4, 349.23)   # F        3/8 ^
seq_arp_2 << SequenceElement(0.125, 0.2, 392)      # G        1/8
seq_arp_2 *= 2

seq_arp_1 << seq_arp_2

arpeggio = Sequencer()
arpeggio << seq_arp_1

hub_vibrato = Hub(comp_type_t.MULTIP)
hub_vibrato << arpeggio.amp_port << lfo_vibrato.output_port

lead.freq_port << arpeggio.freq_port
lead.amp_port << hub_vibrato.output_port

seq_bas = SequenceFunction()
seq_bas << SequenceElement(2, 0.2, 65.406)         # C        2
seq_bas << SequenceElement(2, 0.2, 58.27)          # A#       2

base = Sequencer()
base << seq_bas

hub_wah = Hub(comp_type_t.MULTIP)
hub_wah << base.amp_port << lfo_wah.output_port

bass.freq_port << base.freq_port
bass.amp_port << hub_wah.output_port

hub_master = Hub(comp_type_t.ADD)
#hub_master << lead.output_port << bass.output_port

sound_card << hub_master.output_port

#sound_card.play()