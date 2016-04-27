class Point3D
  def initialize(x,y,z)
    @x = x
    @y = y
    @z = z
  end

  # return Point3D
  def distance(p)
    dx = @x - p.x
    dy = @y - p.y
    dz = @z - p.z
    return Math::sqrt(dx ** 2 + dy ** 2 + dz ** 2)
  end
  
  # 外積
  # return Point3D
  def cross(p)
  	a = @y * p.z - @z * p.y
  	b = @z * p.x - @x * p.z
  	c = @x * p.y - @y * p.x
  	return Point3D.new(a,b,c)
  end
  
  # 内積
  # a*b = axbx + ayby + azbz
  # return Double
  def dot(p)
	return @x * p.x + @y * p.y + @z * p.z
  end

  # ベクトルの大きさ
  # return Double
  def vectorLength
    return Math::sqrt(@x ** 2 + @y ** 2 + @z ** 2)
  end

  # 正規化 : 単位ベクトルに正規化する
  # return Point3D
  def norm
  	point = Point3D.new(@x,@y,@z)
  	len = 1.0 / point.vectorLength
  	@x = @x / len
  	@y = @y / len
  	@z = @z / len
  end
  
  def subtract(p)
    a = @x - p.x
    b = @y - p.y
    c = @z - p.z
    return Point3D.new(a,b,c)
  end
  
  # 右手法か判断
  # 正規化した後、計算する
  # z軸を中心にする。
  # z軸が正の時→x,yが正ならば1
  # z軸が負の時→x,yが負ならば1  を返す
  def systemComparison
    r_value = 0
#    if @x * @y == @z or 
#       @y * @z == @x or 
#       @z * @x == @y then r_value = 1 end
	if @z > 0 and @x > 0 and @y > 0 then return 0 end
	if @z < 0 and @x < 0 and @y < 0 then return 0 end
       
#    puts "#{@x} #{@y} #{@z} "
 
    return r_value
  end
  
  attr_reader :x, :y, :z
end
