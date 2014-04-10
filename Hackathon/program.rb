require 'nokogiri'
require 'open-uri'

class GG
	attr_accessor :Name, :Ar, :item, :id, :level, :diet
	def initialize(name)
		@Name = name
		@Ar = Array.new
	end
end

class Info
	attr_accessor :Restaurant, :Date, :Number
	def initialize()
		@Restaurant = Array.new
		@Date = 0
		@Number = 0
	end
end
	
	
def parser(date, meal)
	url = "http://menu.ha.ucla.edu/foodpro/default.asp?date=#{date}&meal=#{meal}&threshold=2"
	doc = Nokogiri::HTML(open(url))
	data = Info.new()
	data.Date = "#{date}"
	puts data.Date
	data.Number = doc.css("td.menulocheader").length
	puts data.Number
	doc.css("td.menulocheader").each do |thing|
		data.Restaurant.push(GG.new(thing.text)) 
	end
	#data.Restaurant.each do |e|
		#print "#{e.Name}\n"
	#end
	i = 0
	doc.css("td.menugridcell").each do |cell|
		unless cell == nil
			titleli = cell.css("li")[0]
			unless titleli == nil
				data.Restaurant[i % data.Number].Ar.push(GG.new(titleli.text))
				l = data.Restaurant[i % data.Number].Ar.length() - 1
				cell.css("li")[1..-1].each do |h|
					data.Restaurant[i% data.Number].Ar[l].Ar.push(GG.new(h.css("a").text))
					ln = data.Restaurant[i % data.Number].Ar[l].Ar.length() - 1
					data.Restaurant[i% data.Number].Ar[l].Ar[ln].level = h.attr("class")[5..-1]
					data.Restaurant[i% data.Number].Ar[l].Ar[ln].id = h.css("a")[0].attr("onmouseover")[12..-4]
					if h.css("img[alt=Vegan]").length > 0
						data.Restaurant[i% data.Number].Ar[l].Ar[ln].diet = 2
					elsif h.css("img[alt=Vegetarian]").length > 0
						data.Restaurant[i% data.Number].Ar[l].Ar[ln].diet = 1
					else 
						data.Restaurant[i% data.Number].Ar[l].Ar[ln].diet = 0
					end
				end
			end
		end
	i += 1;
	end
	data.Restaurant.each do |a|
		print "#{a.Name}: \n"
		a.Ar.each do |z|
			print "#{z.Name} :\n"
				z.Ar.each do |i|
				print "     #{i.Name} class: #{i.level} diet: #{i.diet} id: #{i.id}\n"
			end
			print "\n"
		end
	#print data.Restaurant[0].Ar[0].Ar[0].diet
	end
	return data
	
end
	
parser("4-21-2012", "2")