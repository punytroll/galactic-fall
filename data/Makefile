all_data = \
	.data/message_label.data \
	.data/time_warp_label.data \
	.data/system_label.data \
	.data/credits_label.data \
	.data/fuel_label.data \
	.data/radar_widget.data \
	.data/mini_map_widget.data \
	.data/target_label.data \
	.data/current_system_label.data

all: data.arx

clean:
	$(RM) -f data.arx
	$(RM) -rf .data

data.arx: .data $(all_data) Makefile
	@$(RM) -f $@
	arxtouch $@
	arxtouch $@ 0
	arxmod $@ 0 -n "Root" -t 0 -st 0 -root
	arxtouch $@ 1
	arxmod $@ 1 -n "User Interface" -t 0 -st 0
	arxmod $@ 0 -a "child" 1
	arxadd $@ --uid 100 --name "Message Label" --type 1 --subtype 1 .data/message_label.data
	arxmod $@ 1 -a "child" 100
	arxadd $@ --uid 101 --name "Time Warp Label" --type 1 --subtype 1 .data/time_warp_label.data
	arxmod $@ 1 -a "child" 101
	arxadd $@ --uid 102 --name "System Label" --type 1 --subtype 1 .data/system_label.data
	arxmod $@ 1 -a "child" 102
	arxadd $@ --uid 103 --name "Credits Label" --type 1 --subtype 1 .data/credits_label.data
	arxmod $@ 1 -a "child" 103
	arxadd $@ --uid 104 --name "Fuel Label" --type 1 --subtype 1 .data/fuel_label.data
	arxmod $@ 1 -a "child" 104
	arxadd $@ --uid 105 --name "Radar Widget" --type 1 --subtype 0 .data/radar_widget.data
	arxmod $@ 1 -a "child" 105
	arxadd $@ --uid 106 --name "Mini Map Widget" --type 1 --subtype 0 .data/mini_map_widget.data
	arxmod $@ 1 -a "child" 106
	arxadd $@ --uid 107 --name "Target Label" --type 1 --subtype 1 .data/target_label.data
	arxmod $@ 1 -a "child" 107
	arxadd $@ --uid 108 --name "Current System Label" --type 1 --subtype 1 .data/current_system_label.data
	arxmod $@ 1 -a "child" 108

.data:
	mkdir $@

.data/%.data: %.xml definitions.xml make_binary.py
	python make_binary.py --definitions=definitions.xml --in=$< --out=$@
