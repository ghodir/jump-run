#ifndef __VOXEL_H__
#define __VOXEL_H__

class Voxel {
	bool solid;

public:
	Voxel() {
		solid = false;
	}
	Voxel(bool solid) {
		this->solid = solid;
	}


	inline bool isSolid() const { return solid; }
	inline void isSolid(bool solid) { this->solid = solid; }
};

#endif