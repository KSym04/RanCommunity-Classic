/*!
 * \file GLTransformSkinSet.h
 *
 * \author Juver
 * \date September 2018
 *
 * 
 */

#ifndef _GLTRANSFORM_SKIN_SET_
#define _GLTRANSFORM_SKIN_SET_


typedef std::map<DWORD,std::string>			TRANSFORM_SKIN_SET_MAP;
typedef TRANSFORM_SKIN_SET_MAP::iterator	TRANSFORM_SKIN_SET_MAP_ITER;

class GLTransformSkinSet
{
private:
	TRANSFORM_SKIN_SET_MAP m_map_skin_set;

public:
	void cleanup();
	void load_file ( std::string file_name );

public:
	GLTransformSkinSet();
	~GLTransformSkinSet();

public:
	static GLTransformSkinSet& GetInstance ();

	
	std::string get_skin( DWORD id );

	TRANSFORM_SKIN_SET_MAP& get_map() { return m_map_skin_set; }
};

#endif