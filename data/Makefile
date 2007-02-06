all_data = \
	.data/message_label.data \
	.data/time_warp_label.data \
	.data/system_label.data \
	.data/credits_label.data \
	.data/fuel_label.data \
	.data/radar_widget.data \
	.data/mini_map_widget.data \
	.data/target_label.data \
	.data/current_system_label.data \
	.data/cargo_cube_model.data \
	.data/shuttle_ship_class.data \
	.data/food_commodity.data \
	.data/industrial_commodity.data \
	.data/medical_supplies_commodity.data \
	.data/luxury_goods_commodity.data \
	.data/metal_commodity.data \
	.data/equipment_commodity.data \
	.data/sol_system.data \
	.data/rigel_system.data \
	.data/tichel_system.data \
	.data/alpha_centauri_system.data \
	.data/shuttle_model.data \
	.data/sol_alpha_centauri_system_link.data \
	.data/sol_rigel_system_link.data \
	.data/tichel_alpha_centauri_system_link.data \
	.data/tichel_rigel_system_link.data

all: data.arx

clean:
	$(RM) -f data.arx
	$(RM) -rf .data
	$(RM) -f shuttle_model.xml
	$(RM) -f cargo_cube_model.xml

data.arx: .data $(all_data) Makefile
	@$(RM) -f $@
	arxtouch $@
	arxtouch $@ 0
	arxmod $@ 0 -n "Root" -t 0 -st 0 -root
	arxtouch $@ 1
	arxmod $@ 1 -n "User Interface" -t 0 -st 0
	arxmod $@ 0 -a "child" 1
	arxtouch $@ 2
	arxmod $@ 2 -n "Models" -t 0 -st 0
	arxmod $@ 0 -a "child" 2
	arxtouch $@ 3
	arxmod $@ 3 -n "Ship Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 3
	arxtouch $@ 4
	arxmod $@ 4 -n "Commodities" -t 0 -st 0
	arxmod $@ 0 -a "child" 4
	arxtouch $@ 5
	arxmod $@ 5 -n "Systems" -t 0 -st 0
	arxmod $@ 0 -a "child" 5
	arxtouch $@ 6
	arxmod $@ 6 -n "System Links" -t 0 -st 0
	arxmod $@ 0 -a "child" 6
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
	arxadd $@ --uid 109 --name "Cargo Cube Model" --type 2 --subtype 0 .data/cargo_cube_model.data
	arxmod $@ 2 -a "child" 109
	arxadd $@ --uid 110 --name "Shuttle Ship Class" --type 3 --subtype 0 .data/shuttle_ship_class.data
	arxmod $@ 3 -a "child" 110
	arxadd $@ --uid 111 --name "Food Commodity" --type 4 --subtype 0 .data/food_commodity.data
	arxmod $@ 4 -a "child" 111
	arxadd $@ --uid 112 --name "Industrial Commodity" --type 4 --subtype 0 .data/industrial_commodity.data
	arxmod $@ 4 -a "child" 112
	arxadd $@ --uid 113 --name "Medical Supplies Commodity" --type 4 --subtype 0 .data/medical_supplies_commodity.data
	arxmod $@ 4 -a "child" 113
	arxadd $@ --uid 114 --name "Luxury Goods Commodity" --type 4 --subtype 0 .data/luxury_goods_commodity.data
	arxmod $@ 4 -a "child" 114
	arxadd $@ --uid 115 --name "Metal Commodity" --type 4 --subtype 0 .data/metal_commodity.data
	arxmod $@ 4 -a "child" 115
	arxadd $@ --uid 116 --name "Equipment Commodity" --type 4 --subtype 0 .data/equipment_commodity.data
	arxmod $@ 4 -a "child" 116
	arxadd $@ --uid 117 --name "Sol System" --type 5 --subtype 0 .data/sol_system.data
	arxmod $@ 5 -a "child" 117
	arxadd $@ --uid 118 --name "Rigel System" --type 5 --subtype 0 .data/rigel_system.data
	arxmod $@ 5 -a "child" 118
	arxadd $@ --uid 119 --name "Tichel System" --type 5 --subtype 0 .data/tichel_system.data
	arxmod $@ 5 -a "child" 119
	arxadd $@ --uid 120 --name "Alpha Centauri System" --type 5 --subtype 0 .data/alpha_centauri_system.data
	arxmod $@ 5 -a "child" 120
	arxadd $@ --uid 121 --name "Shuttle Model" --type 2 --subtype 0 .data/shuttle_model.data
	arxmod $@ 2 -a "child" 121
	arxadd $@ --uid 122 --name "Sol / Alpha Centauri System Link" --type 6 --subtype 0 .data/sol_alpha_centauri_system_link.data
	arxmod $@ 6 -a "child" 122
	arxadd $@ --uid 123 --name "Sol / Rigel System Link" --type 6 --subtype 0 .data/sol_rigel_system_link.data
	arxmod $@ 6 -a "child" 123
	arxadd $@ --uid 124 --name "Tichel / Alpha Centauri System Link" --type 6 --subtype 0 .data/tichel_alpha_centauri_system_link.data
	arxmod $@ 6 -a "child" 124
	arxadd $@ --uid 125 --name "Tichel / Rigel System Link" --type 6 --subtype 0 .data/tichel_rigel_system_link.data
	arxmod $@ 6 -a "child" 125

.data:
	mkdir $@

.data/%.data: %.xml definitions.xml make_binary.py
	python make_binary.py --definitions=definitions.xml --in=$< --out=$@

shuttle_model.xml: models/shuttle_model.xml model_transform.xsl
	xsltproc -output $@ model_transform.xsl $<

cargo_cube_model.xml: models/cargo_cube_model.xml model_transform.xsl
	xsltproc -output $@ model_transform.xsl $<
