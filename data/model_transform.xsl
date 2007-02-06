<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/model">
	<model>
		<identifier><xsl:value-of select="@identifier"/></identifier>
		<points>
			<xsl:apply-templates select="point"/>
		</points>
		<triangle-points>
			<xsl:apply-templates select="triangle-point"/>
		</triangle-points>
		<triangles>
			<xsl:apply-templates select="triangle"/>
		</triangles>
	</model>
</xsl:template>

<xsl:template match="/model/point">
	<point>
		<identifier><xsl:value-of select="@identifier"/></identifier>
		<name><xsl:value-of select="@name"/></name>
		<position>
			<x><xsl:value-of select="@position-x"/></x>
			<y><xsl:value-of select="@position-y"/></y>
			<z><xsl:value-of select="@position-z"/></z>
		</position>
	</point>
</xsl:template>

<xsl:template match="/model/triangle-point">
	<triangle-point>
		<identifier><xsl:value-of select="@identifier"/></identifier>
		<normal>
			<x><xsl:value-of select="@normal-x"/></x>
			<y><xsl:value-of select="@normal-y"/></y>
			<z><xsl:value-of select="@normal-z"/></z>
		</normal>
		<point-identifier><xsl:value-of select="point/@point-identifier"/></point-identifier>
	</triangle-point>
</xsl:template>

<xsl:template match="/model/triangle">
	<triangle>
		<identifier><xsl:value-of select="@identifier"/></identifier>
		<name><xsl:value-of select="@name"/></name>
		<triangle-point-1-identifier><xsl:value-of select="triangle-point[1]/@triangle-point-identifier"/></triangle-point-1-identifier>
		<triangle-point-2-identifier><xsl:value-of select="triangle-point[2]/@triangle-point-identifier"/></triangle-point-2-identifier>
		<triangle-point-3-identifier><xsl:value-of select="triangle-point[3]/@triangle-point-identifier"/></triangle-point-3-identifier>
	</triangle>
</xsl:template>

</xsl:stylesheet>
