require './point3D.rb'
require './face.rb'

## incremental convexhull

# 3D convex hull
# param: points the Point3D coord
# return: a series of indices to create the faces of the hull

def convexhull(points)
	if points.length < 3 then return nil end
	
	face = Face.new(points)
	face.vertices(0,1,2)
	
	# 最初に4面体を作り、計算をする。
	# 頂点が4つ未満の場合、nullを返す
	v = centroid(points, 3, face)
	
	if face.isVisible(v) then face.flip end
	
	face0 = Face.new(points)
	face0.vertices(3, face.i0, face.i1)
	if face0.isVisible(v) then face0.flip end
	
	face1 = Face.new(points)
	face1.vertices(3, face.i1, face.i2)
	if face1.isVisible(v) then face1.flip end

	face2 = Face.new(points)
	face2.vertices(3, face.i2, face.i0)
	if face2.isVisible(v) then face2.flip end
	
	# 4面体を形成する
	valid_faces = [face,face0,face1,face2]
	visible_faces = []
	tmp_faces = []
	result = Hash::new
	
	# すべての頂点に処理を掛ける。最初の4つの点は省く
	(4..points.length-1).each{ |i|
		v = points[i]
		
		visible_faces = []
		
		valid_faces.each{ |face|
			if face.isVisible(v) then
				visible_faces << face
			end
		}
		
		# 4面体の頂点の中にあるため、次の点で比較
		if visible_faces.length == 0 then next end
		
		# 頂点が凸砲の外側にある場合、リストから表示されているすべての面を削除
		visible_faces.each{ |face|
			delete_id = valid_faces.index(face)
			valid_faces.delete_at(delete_id)
		}
		
		# 片面だけの場合、3面で構成しても大丈夫なので3面で構成
		if visible_faces.length == 1 then
			face = visible_faces[0]
			input_face = Face.new(points)
			input_face.vertices(i, face.i0, face.i1)
			valid_faces << input_face
			
			input_face = Face.new(points)
			input_face.vertices(i, face.i1, face.i2)
			valid_faces << input_face
			
			input_face = Face.new(points)
			input_face.vertices(i, face.i2, face.i0)
			valid_faces << input_face
			next
		end
		
		# すべての頂点が既存のものより外側にあった場合、すべての面から新しく作る
		tmp_faces = []
		count = 0
		
		# 新しいvisible_facesを作成する
		visible_faces.each{ |face|
			input_face = Face.new(points)
			
			input_face.vertices(i, face.i0, face.i1)
			tmp_faces << input_face
			
			input_face = Face.new(points)
			input_face.vertices(i, face.i1, face.i2)
			tmp_faces << input_face
			
			input_face = Face.new(points)
			input_face.vertices(i, face.i2, face.i0)
			tmp_faces << input_face
			count = count + 1
		}
		
		
		tmp_faces.each{ |face|
			flag = true
			
			# 面の前に点がないかを検索する
			tmp_faces.each{ |other|
				if flag then
					if face != other then
						if face.isVisible(other.centroid) then
							face = nil
							break
							flag = false
						end
					end
				end
			}
			
			# 面の前に点が無い場合格納する
			if !face.nil? then
				valid_faces << face
			end
		}
		
	}
	
	h_count = 0
	valid_faces.each{ |face|
		input_array = [face.i0, face.i1, face.i2]
		result[h_count] = input_array
		h_count = h_count + 1
	}
	
	# 3D covex hullの結果を返す
	return result

end

def centroid(points, index, face)
	p = points[index]
	p0 = points[face.i0]
	p1 = points[face.i1]
	p2 = points[face.i2]
	return Point3D.new( (p.x + p0.x + p1.x + p2.x) / 4, (p.y + p0.y + p1.y + p2.y) / 4, (p.z + p0.z + p1.z + p2.z) / 4)
end


point = []

infile = ARGV[0]
outfile = ARGV[1]

open(infile,'r'){|strs|
  while line = strs.gets
  	str = line.split(" ")
  	coord = Point3D.new(str[0].to_f,str[1].to_f, str[2].to_f)
  	point << coord
  end
}

puts point.size

hull = convexhull(point)
points = []
(0..10).each{|i|
	points << point[i]
}


# 結果を表示
#p hull

# 結果をファイルに書き込む
out = ""
open(infile, "r") {|f|
	while line = f.gets
		out << "v " << line
	end
}
hull.each do |k, v|
	out << "f" << " "\
	    << (v[0] + 1).to_s << " "\
	    << (v[1] + 1).to_s << " "\
	    << (v[2] + 1).to_s << "\n"
end

File.write(outfile, out)
