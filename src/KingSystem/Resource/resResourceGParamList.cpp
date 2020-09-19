#include "KingSystem/Resource/resResourceGParamList.h"
#include <KingSystem/ActorSystem/actActorParamMgr.h>
#include <agl/Utils/aglParameter.h>
#include <prim/seadRuntimeTypeInfo.h>
#include "KingSystem/Resource/GeneralParamList/resGParamListObject.h"
#include "KingSystem/Resource/GeneralParamList/resGParamListTraits.h"

namespace ksys::res {

void GParamList::doCreate_(u8*, u32, sead::Heap*) {}

template <GParamListObjType Type>
KSYS_ALWAYS_INLINE void GParamList::add(const agl::utl::ResParameterList& list,
                                        const sead::SafeString& name, sead::Heap* heap,
                                        GParamList* dummy_list) {
    const auto pobj = agl::utl::getResParameterObj(list, name);
    if (pobj.ptr()) {
        using T = typename GParamListObjTypeTraits<Type>::type;

        auto* obj = new (heap) T;
        if (obj)
            addObj(&obj->getObj(), obj->getName());

        mObjects[s32(Type)] = obj;

    } else {
        mObjects[s32(Type)] = dummy_list ? dummy_list->mObjects[s32(Type)] : nullptr;
    }
}

bool GParamList::parse_(u8* data, size_t, sead::Heap* heap) {
    GParamList* dummy_list = nullptr;
    if (!sead::IsDerivedFrom<DummyGParamList>(this) && act::ActorParamMgr::instance())
        dummy_list = act::ActorParamMgr::instance()->getDummyGParamList();

    mObjects.allocBufferAssert(NumGParamListObjTypes, heap);

    const agl::utl::ResParameterArchive archive{data};

    // TODO: System, etc.

    if (data)
        applyResParameterArchive(archive);

    return true;
}

void GParamList::finalize_() {}

void DummyGParamList::doCreate_(u8*, u32, sead::Heap*) {}

bool DummyGParamList::parse_(u8* data, size_t size, sead::Heap* heap) {
    GParamList::parse_(data, size, heap);
    return true;
}

}  // namespace ksys::res
