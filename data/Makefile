all: data.arx

data.arx: .data .data/message_label.data
	@$(RM) -f $@
	arxtouch $@
	arxtouch $@ 0
	arxmod $@ 0 -n "Root" -t 0 -st 0 -root
	arxtouch $@ 1
	arxmod $@ 1 -n "User Interface" -t 0 -st 0
	arxmod $@ 0 -a "child" 1
	arxadd $@ --uid 100 --name "Message Label" --type 1 --subtype 0 .data/message_label.data
	arxmod $@ 1 -a "child" 100

.data:
	mkdir $@

.data/message_label.data: message_label.xml
	python make_binary.py --definitions=definitions.xml --in=$< --out=$@
