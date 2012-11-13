class List
	Box = Struct.new(:value, :previous, :next)
	@@index = 0

	def create
		@list = []
		@node = []
		@previous = 0
		@next = 2
	end

	def add(something)
		@value = something		
		@list << Box.new(@value, @previous, @next)
		if @@index == 0
			@node[@@index] = [@the_next = @next,@the_previous = nil]
		else
			@node[@@index - 1] = [@the_next = @the_next, @the_previous = @previous]
			@node[@@index] = [@the_next = @next,@the_previous = nil]
		end
		@previous += 1
		@next += 1
		@@index += 1
	end



	def output_list
		puts @list
		puts " "
		p @node
	end

end
f = List.new 
f.create
f.add("Hello")
f.add("white")
f.add("rabbit")
f.output_list

