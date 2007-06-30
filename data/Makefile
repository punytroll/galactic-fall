all_data = \
	.data/commodity_equipment.data \
	.data/commodity_food.data \
	.data/commodity_fuel.data \
	.data/commodity_industrial.data \
	.data/commodity_luxury_goods.data \
	.data/commodity_medical_supplies.data \
	.data/commodity_metal.data \
	.data/label_credits.data \
	.data/label_current_system.data \
	.data/label_fuel.data \
	.data/label_hull.data \
	.data/label_message.data \
	.data/label_system.data \
	.data/label_target.data \
	.data/label_time_warp.data \
	.data/model_cargo_cube.data \
	.data/model_fighter.data \
	.data/model_laser_shot.data \
	.data/model_planet.data \
	.data/model_shuttle.data \
	.data/model_transporter.data \
	.data/ship_class_shuttle.data \
	.data/ship_class_transporter.data \
	.data/ship_class_fighter.data \
	.data/system_alpha_centauri.data \
	.data/system_rigel.data \
	.data/system_sol.data \
	.data/system_tichel.data \
	.data/system_link_alpha_centauri_sol.data \
	.data/system_link_alpha_centauri_tichel.data \
	.data/system_link_rigel_sol.data \
	.data/system_link_rigel_tichel.data \
	.data/weapon_class_light_laser.data \
	.data/weapon_class_medium_laser.data \
	.data/widget_mini_map.data \
	.data/widget_mini_map_display.data \
	.data/widget_scanner.data \
	.data/widget_scanner_display.data

raw_to_littlem_dependencies = \
	raw_to_littlem.py \
	xml_stream.py

all: check-dependencies data.arx

check-dependencies:
	@if [ x`which arxtouch` = x -o x`which arxmod` = x -o x`which arxadd` = x ]; then \
	    echo -e "\nYour system is missing the arxtools suite.\nYou will need to install this package on your system\nor download it from http://libarxx.sourceforge.net and install it.\n"; false; \
	fi
	@if [ x`which python` = x ]; then \
	    echo -e "\nYour system is missing the python interpreter.\nIn order to build the data file you will need to install it on your system.\n"; false; \
	fi
	@if [ x`which xsltproc` = x ]; then \
	    echo -e "\nYour system is missing the xsltproc program.\nTo build the data file you will need to install it on your system.\n"; false; \
	fi

clean:
	$(RM) -rf .data
	$(RM) -f data.arx
	$(RM) -f xml_stream.pyc
	$(RM) -f model_cargo_cube.xml
	$(RM) -f model_fighter.xml
	$(RM) -f model_laser_shot.xml
	$(RM) -f model_planet.xml
	$(RM) -f model_shuttle.xml
	$(RM) -f model_transporter.xml
	$(RM) -f models/fighter.xml
	$(RM) -f models/planet.xml
	$(RM) -f models/shuttle.xml
	$(RM) -f models/transporter.xml

data.arx: .data $(all_data) Makefile
	@$(RM) -f $@
	@echo -e "\n\nPreparing directory structure ...\n"
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
	arxtouch $@ 7
	arxmod $@ 7 -n "Weapon Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 7
	@echo -e "\n\nAdding widgets ...\n"
	arxadd $@ --uid 103 --name "Credits" --type 1 --subtype 1 --add 1:child .data/label_credits.data
	arxadd $@ --uid 108 --name "Current System" --type 1 --subtype 1 --add 1:child .data/label_current_system.data
	arxadd $@ --uid 104 --name "Fuel" --type 1 --subtype 1 --add 1:child .data/label_fuel.data
	arxadd $@ --uid 132 --name "Hull" --type 1 --subtype 1 --add 1:child .data/label_hull.data
	arxadd $@ --uid 100 --name "Message" --type 1 --subtype 1 --add 1:child .data/label_message.data
	arxadd $@ --uid 102 --name "System" --type 1 --subtype 1 --add 1:child .data/label_system.data
	arxadd $@ --uid 107 --name "Target" --type 1 --subtype 1 --add 1:child .data/label_target.data
	arxadd $@ --uid 101 --name "Time Warp" --type 1 --subtype 1 --add 1:child .data/label_time_warp.data
	arxadd $@ --uid 106 --name "Mini Map" --type 1 --subtype 0 --add 1:child .data/widget_mini_map.data
	arxadd $@ --name "Mini Map Display" --type 1 --subtype 2 --add 1:child .data/widget_mini_map_display.data
	arxadd $@ --uid 105 --name "Scanner" --type 1 --subtype 0 --add 1:child .data/widget_scanner.data
	arxadd $@ --uid 126 --name "Scanner Display" --type 1 --subtype 3 --add 1:child .data/widget_scanner_display.data
	@echo -e "\n\nAdding weapon classes ...\n"
	arxadd $@ --name "Light Laser" --type 7 --subtype 0 --add 7:child .data/weapon_class_light_laser.data
	arxadd $@ --name "Medium Laser" --type 7 --subtype 0 --add 7:child .data/weapon_class_medium_laser.data
	@echo -e "\n\nAdding ship classes ...\n"
	arxadd $@ --uid 131 --name "Fighter" --type 3 --subtype 0 --add 3:child .data/ship_class_fighter.data
	arxadd $@ --uid 110 --name "Shuttle" --type 3 --subtype 0 --add 3:child .data/ship_class_shuttle.data
	arxadd $@ --uid 128 --name "Transporter" --type 3 --subtype 0 --add 3:child .data/ship_class_transporter.data
	@echo -e "\n\nAdding commodities ...\n"
	arxadd $@ --uid 116 --name "Equipment" --type 4 --subtype 0 --add 4:child .data/commodity_equipment.data
	arxadd $@ --uid 111 --name "Food" --type 4 --subtype 0 --add 4:child .data/commodity_food.data
	arxadd $@ --name "Fuel" --type 4 --subtype 0 --add 4:child .data/commodity_fuel.data
	arxadd $@ --uid 112 --name "Industrial" --type 4 --subtype 0 --add 4:child .data/commodity_industrial.data
	arxadd $@ --uid 114 --name "Luxury Goods" --type 4 --subtype 0 --add 4:child .data/commodity_luxury_goods.data
	arxadd $@ --uid 113 --name "Medical Supplies" --type 4 --subtype 0 --add 4:child .data/commodity_medical_supplies.data
	arxadd $@ --uid 115 --name "Metal" --type 4 --subtype 0 --add 4:child .data/commodity_metal.data
	@echo -e "\n\nAdding systems ...\n"
	arxadd $@ --uid 120 --name "Alpha Centauri" --type 5 --subtype 0 --add 5:child .data/system_alpha_centauri.data
	arxadd $@ --uid 118 --name "Rigel" --type 5 --subtype 0 --add 5:child .data/system_rigel.data
	arxadd $@ --uid 117 --name "Sol" --type 5 --subtype 0 --subtype 0 --add 5:child .data/system_sol.data
	arxadd $@ --uid 119 --name "Tichel" --type 5 --subtype 0 --add 5:child .data/system_tichel.data
	@echo -e "\n\nAdding system links ...\n"
	arxadd $@ --uid 122 --name "Alpha Centauri / Sol" --type 6 --subtype 0 --add 6:child .data/system_link_alpha_centauri_sol.data
	arxadd $@ --uid 124 --name "Alpha Centauri / Tichel" --type 6 --subtype 0 --add 6:child .data/system_link_alpha_centauri_tichel.data
	arxadd $@ --uid 123 --name "Rigel / Sol" --type 6 --subtype 0 --add 6:child .data/system_link_rigel_sol.data
	arxadd $@ --uid 125 --name "Rigel / Tichel" --type 6 --subtype 0 --add 6:child .data/system_link_rigel_tichel.data
	@echo -e "\n\nAdding models ...\n"
	arxadd $@ --uid 109 --name "Cargo Cube" --type 2 --subtype 0 --add 2:child .data/model_cargo_cube.data
	arxadd $@ --uid 130 --name "Fighter" --type 2 --subtype 0 --add 2:child .data/model_fighter.data
	arxadd $@ --name "Laser Shot" --type 2 --subtype 0 --add 2:child .data/model_laser_shot.data
	arxadd $@ --uid 129 --name "Planet" --type 2 --subtype 0 --add 2:child .data/model_planet.data
	arxadd $@ --uid 121 --name "Shuttle" --type 2 --subtype 0 --add 2:child .data/model_shuttle.data
	arxadd $@ --uid 127 --name "Transporter" --type 2 --subtype 0 --add 2:child .data/model_transporter.data

.data:
	mkdir $@

.data/%.data: %.xml definitions.xml xml_to_data.py
	python xml_to_data.py --definitions=definitions.xml --in=$< --out=$@

model_cargo_cube.xml: models/cargo_cube.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

model_fighter.xml: models/fighter.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

model_laser_shot.xml: models/laser_shot.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

model_planet.xml: models/planet.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

model_shuttle.xml: models/shuttle.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

model_transporter.xml: models/transporter.xml littlem_to_xml.xsl
	xsltproc -output $@ littlem_to_xml.xsl $<

models/fighter.xml: models/fighter.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=fighter

models/planet.xml: models/planet.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=planet

models/shuttle.xml: models/shuttle.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=shuttle

models/transporter.xml: models/transporter.raw $(raw_to_littlem_dependencies)
	python raw_to_littlem.py --in=$< --out=$@ --identifier=transporter

.PHONY: check-dependencies
