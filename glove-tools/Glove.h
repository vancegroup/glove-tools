// Internal Includes

// Library/third-party includes

// Standard includes
#include <vector>



class Glove
{
	public:
		OSG::Group getNode() const;
		void updateData();
		float getBend(int finger);
		
	private:
		OSG::ref_ptr<GloveNode> _node;
		scoped_ptr<IGloveHardware> _hardware;
		vector<float, 5> _bends;
}