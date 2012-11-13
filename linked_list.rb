class List
	@@k = 0
	attr :list, true
	def create_list
		@list = []	
	end

	def output_list
		p @list
		puts "/////////////////////////////////////////////////"
	end

	def output_element(index)
		puts @list[index - 1]
		puts "/////////////////////////////////////////////////"
	end

	def add(something)
		@list << something
		@@k += 1 
	end

	def remove(index)
		remove = []	
		remove << @list[index - 1]
		@list = @list - remove
		@@k -= 1
	end

	def replace(index, something )	
		@list[index - 1] = something
	end

	def previous (index)
		if index - 1 == 0
		puts "Element is the first!"
		elsif index - 1 < 0 || index - 1 > @@k
		puts "Element with the index does not exist!"
		else puts @list[index - 2]
		end
		puts "/////////////////////////////////////////////////"
	end

	def the_next(index)
		if index - 1 == @@k - 1
		puts "Element is the last!"
		elsif index - 1 < 0 || index - 1 > @@k
		puts "Element with the index does not exist!"
		else puts @list[index]
		end
		puts "/////////////////////////////////////////////////"
	end
end
h = List.new
h.create_list
h.add("Hello")
h.add("white")
h.add("rabbit!")
h.output_list
h.output_element(1)
h.replace(1, "Goodbye")
h.output_list
h.previous(2)
h.the_next(2)
h.remove(2)
h.output_list
