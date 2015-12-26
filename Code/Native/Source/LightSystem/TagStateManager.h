#ifndef TAG_STATE_MANAGER_H
#define TAG_STATE_MANAGER_H

#include "pandabase.h"
#include "bitMask.h"
#include "camera.h"
#include "nodePath.h"
#include "shader.h"
#include "renderState.h"
#include "shaderAttrib.h"
#include "colorWriteAttrib.h"

NotifyCategoryDecl(tagstatemgr, EXPORT_CLASS, EXPORT_TEMPL);

/**
 * @brief This class handles all different tag states
 * @details The TagStateManager stores a list of RenderStates assigned to different
 *   steps in the pipeline. For example, there are a list of shadow states, which
 *   are applied whenever objects are rendered from a shadow camera.
 *   
 *   The Manager also stores a list of all cameras used in the different stages,
 *   to keep track of the states used and to be able to attach new states.
 */
class TagStateManager {

    PUBLISHED:
        TagStateManager(NodePath main_cam_node);
        ~TagStateManager();

        inline static BitMask32 get_gbuffer_mask();
        inline static BitMask32 get_voxelize_mask();
        inline static BitMask32 get_shadow_mask();

        inline void apply_shadow_state(NodePath np, Shader* shader, const string &name, int sort);
        inline void apply_voxelize_state(NodePath np, Shader* shader, const string &name, int sort);
        void cleanup_states();
        
        inline void register_shadow_camera(Camera* source);
        inline void unregister_shadow_camera(Camera* source);

        inline void register_voxelize_camera(Camera* source);
        inline void unregister_voxelize_camera(Camera* source);

    private:
        
        typedef vector<Camera*> CameraList;
        typedef pmap<string, CPT(RenderState)> TagStateList;

        struct StateContainer {
            CameraList cameras;
            TagStateList tag_states;
            string tag_name;
            BitMask32 mask;
        };

        void apply_state(StateContainer& container, NodePath np, Shader* shader,
                         const string& name, int sort);
        void cleanup_states(StateContainer& container);
        void register_camera(StateContainer &container, Camera* source);
        void unregister_camera(StateContainer &container, Camera* source);

        StateContainer _shadow_container;
        StateContainer _voxelize_container;

        NodePath _main_cam_node;
};


#include "TagStateManager.I"

#endif // TAG_STATE_MANAGER_H
