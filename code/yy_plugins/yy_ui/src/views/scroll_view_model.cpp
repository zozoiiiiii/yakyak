#include "scroll_view_model.h"

NS_YY_BEGIN
void ScrollViewModel::Insert(const YY::VarList& args)
{
	int nCnt = args.GetCount();
	for(int i=0; i<nCnt; i++)
		m_objects.push_back(args.GetInt64(i)); 

	GetEventMgr()->Invoke(GetID(), "DataChanged");
	//emit countChanged(ListModelBase::count());
}

void ScrollViewModel::Remove(const YY::VarList& args)
{
	//m_objects.push_back(id);


	//emit countChanged(ListModelBase::count());
}

void ScrollViewModel::Update(const YY::VarList& args)
{
	//m_objects.push_back(id);


	//emit dataChanged(createIndex(count, 0), createIndex(count, 0), m_roleIndexs);
}

NS_YY_END