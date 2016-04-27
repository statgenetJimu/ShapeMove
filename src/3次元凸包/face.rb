require './point3D.rb'

class Face
	def initialize(point)
		@points = point
	end
	
	def vertices(i0, i1, i2)
		@i0 = i0
		@i1 = i1
		@i2 = i2
		
		computePanel
	end
	
	def computePanel
		p1 = @points[@i0]
		p2 = @points[@i1]
		p3 = @points[@i2]
		
		@a = p1.y * (p2.z - p3.z) + p2.y * (p3.z - p1.z) + p3.y * (p1.z - p2.z)
		@b = p1.z * (p2.x - p3.x) + p2.z * (p3.x - p1.x) + p3.z * (p1.x - p2.x)
		@c = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)
		@d = -(p1.x * (p2.y * p3.z - p3.y * p2.z) + p2.x * (p3.y * p1.z - p1.y * p3.z) + p3.x * (p1.y * p2.z - p2.y * p1.z))
		
	end
	
	def isVisible(point)
		return (@a * point.x + @b * point.y + @c * point.z + @d) > 0
	end
	
	def outputtest(point)
		puts "a::#{@a} b::#{@b} c::#{@c} d::#{@d}"
		puts "x::#{point.x} y::#{point.y} z::#{point.z}"
		return @a * point.x + @b * point.y + @c * point.z + @d
	end
	
	def centroid
		p0 = @points[@i0]
		p1 = @points[@i1]
		p2 = @points[@i2]
		return Point3D.new( (p0.x + p1.x + p2.x)/3.0, (p0.y + p1.y + p2.y)/3.0, (p0.z + p1.z + p2.z)/3.0)
	end
	
	def flip
		t = @i0
		@i0 = @i1
		@i1 = t
		computePanel
	end
	attr_reader :points, :i0, :i1, :i2
end
