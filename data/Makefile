all_data = \
	.data/message_label.data \
	.data/time_warp_label.data \
	.data/system_label.data \
	.data/credits_label.data \
	.data/fuel_label.data \
	.data/scanner_widget.data \
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
	.data/tichel_rigel_system_link.data \
	.data/scanner_display_widget.data \
	.data/transporter_model.data \
	.data/transporter_ship_class.data \
	.data/planet_model.data \
	.data/fighter_model.data \
	.data/fighter_ship_class.data

raw_to_littlem_dependencies = \
	raw_to_littlem.py \
	xml_stream.py

all: data.arx

clean:
	$(RM) -f data.arx
	$(RM) -rf .data
	$(RM) -f shuttle_model.xml
	$(RM) -f cargo_cube_model.xml
	$(RM) -f transporter_model.xml
	$(RM) -f models/transporter_model.xml
	$(RM) -f planet_model.xml
	$(RM) -f models/planet_model.xml
	$(RM) -f models/fighter_model.xml
	$(RM) -f fighter_model.xml
	$(RM) -f xml_stream.pyc

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
	@echo -e "\n\nAdding widgets ...\n"
	arxadd $@ --uid 100 --name "Message Label" --type 1 --subtype 1 --add 1:child .data/message_label.data
	arxadd $@ --uid 101 --name "Time Warp Label" --type 1 --subtype 1 --add 1:child .data/time_warp_label.data
	arxadd $@ --uid 102 --name "System Label" --type 1 --subtype 1 --add 1:child .data/system_label.data
	arxadd $@ --uid 103 --name "Credits Label" --type 1 --subtype 1 --add 1:child .data/credits_label.data
	arxadd $@ --uid 104 --name "Fuel Label" --type 1 --subtype 1 --add 1:child .data/fuel_label.data
	arxadd $@ --uid 105 --name "Scanner Widget" --type 1 --subtype 0 --add 1:child .data/scanner_widget.data
	arxadd $@ --uid 106 --name "Mini Map Widget" --type 1 --subtype 0 --add 1:child .data/mini_map_widget.data
	arxadd $@ --uid 107 --name "Target Label" --type 1 --subtype 1 --add 1:child .data/target_label.data
	arxadd $@ --uid 108 --name "Current System Label" --type 1 --subtype 1 --add 1:child .data/current_system_label.data
	arxadd $@ --uid 126 --name "Scanner Display Widget" --type 1 --subtype 0 --add 1:child .data/scanner_display_widget.data
	@echo -e "\n\nAdding models ...\n"
	arxadd $@ --uid 127 --name "Transporter Model" --type 2 --subtype 0 --add 2:child .data/transporter_model.data
	arxadd $@ --uid 129 --name "Planet Model" --type 2 --subtype 0 --add 2:child .data/planet_model.data
	arxadd $@ --uid 130 --name "Fighter Model" --type 2 --subtype 0 --add 2:child .data/fighter_model.data
	arxadd $@ --uid 121 --name "Shuttle Model" --type 2 --subtype 0 --add 2:child .data/shuttle_model.data
	arxadd $@ --uid 109 --name "Cargo Cube Model" --type 2 --subtype 0 --add 2:child .data/cargo_cube_model.data
	@echo -e "\n\nAdding ship classes ...\n"
	arxadd $@ --uid 131 --name "Fighter Ship Class" --type 3 --subtype 0 --add 3:child .data/fighter_ship_class.data
	arxadd $@ --uid 110 --name "Shuttle Ship Class" --type 3 --subtype 0 --add 3:child .data/shuttle_ship_class.data
	arxadd $@ --uid 128 --name "Transporter Ship Class" --type 3 --subtype 0 --add 3:child .data/transporter_ship_class.data
	@echo -e "\n\nAdding commodities ...\n"
	arxadd $@ --uid 116 --name "Equipment Commodity" --type 4 --subtype 0 --add 4:child .data/equipment_commodity.data
	arxadd $@ --uid 111 --name "Food Commodity" --type 4 --subtype 0 --add 4:child .data/food_commodity.data
	arxadd $@ --uid 112 --name "Industrial Commodity" --type 4 --subtype 0 --add 4:child .data/industrial_commodity.data
	arxadd $@ --uid 114 --name "Luxury Goods Commodity" --type 4 --subtype 0 --add 4:child .data/luxury_goods_commodity.data
	arxadd $@ --uid 113 --name "Medical Supplies Commodity" --type 4 --subtype 0 --add 4:child .data/medical_supplies_commodity.data
	arxadd $@ --uid 115 --name "Metal Commodity" --type 4 --subtype 0 --add 4:child .data/metal_commodity.data
	@echo -e "\n\nAdding systems ...\n"
	arxadd $@ --uid 120 --name "Alpha Centauri System" --type 5 --subtype 0 --add 5:child .data/alpha_centauri_system.data
	arxadd $@ --uid 118 --name "Rigel System" --type 5 --subtype 0 --add 5:child .data/rigel_system.data
	arxadd $@ --uid 117 --name "Sol System" --type 5 --subtype 0 --subtype 0 --add 5:child .data/sol_system.data
	arxadd $@ --uid 119 --name "Tichel System" --type 5 --subtype 0 --add 5:child .data/tichel_system.data
	@echo -e "\n\nAdding system links ...\n"
	arxadd $@ --uid 122 --name "Sol / Alpha Centauri System Link" --type 6 --subtype 0 --add 6:child .data/sol_alpha_centauri_system_link.data
	arxadd $@ --uid 123 --name "Sol / Rigel System Link" --type 6 --subtype 0 --add 6:child .data/sol_rigel_system_link.data
	arxadd $@ --uid 124 --name "Tichel / Alpha Centauri System Link" --type 6 --subtype 0 --add 6:child .data/tichel_alpha_centauri_system_link.data
	arxadd $@ --uid 125 --name "Tichel / Rigel System Link" --type 6 --subtype 0 --add 6:child .data/tichel_rigel_system_link.data

.data:
	mkdir $@

.data/%.data: %.xml definitions.xml xml_to_data.py
	python xml_to_data.py --definitions=definitions.xml --in=$< --out=$@

shuttle_model.xml: models/shuttle_model.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

cargo_cube_model.xml: models/cargo_cube_model.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

transporter_model.xml: models/transporter_model.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

planet_model.xml: models/planet_model.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

fighter_model.xml: models/fighter_model.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

models/transporter_model.xml: models/transporter_model.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=transporter

models/planet_model.xml: models/planet_model.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=planet

models/fighter_model.xml: models/fighter_model.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=fighter
