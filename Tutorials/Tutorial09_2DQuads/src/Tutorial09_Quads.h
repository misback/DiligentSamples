/*     Copyright 2015-2018 Egor Yusov
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#pragma once 

#include <atomic>
#include "SampleBase.h"
#include "BasicMath.h"
#include "LockHelper.h"

class Tutorial09_Quads : public SampleBase
{
public:
    ~Tutorial09_Quads()override;
    virtual void GetEngineInitializationAttribs(Diligent::DeviceType DevType, 
                                                Diligent::EngineCreationAttribs &Attribs, 
                                                Diligent::Uint32 &NumDeferredContexts)override;
    virtual void Initialize(Diligent::IRenderDevice *pDevice, 
                            Diligent::IDeviceContext **ppContexts, 
                            Diligent::Uint32 NumDeferredCtx, 
                            Diligent::ISwapChain *pSwapChain)override;
    virtual void Render()override;
    virtual void Update(double CurrTime, double ElapsedTime)override;
    virtual const Diligent::Char* GetSampleName()const override{return "Tutorial09: Quads";}

private:
    static void SetNumQuads(const void *value, void * clientData);
    static void GetNumQuads(void *value, void * clientData);
    static void SetWorkerThreadCount(const void *value, void * clientData);
    static void GetWorkerThreadCount(void *value, void * clientData);
    void InitializeInstanceData();
    void UpdateInstanceData(float elapsedTime);
    void StartWorkerThreads();
    void StopWorkerThreads();
    void RenderSubset(Diligent::IDeviceContext *pCtx, Diligent::Uint32 Subset);

    static void WorkerThreadFunc(Tutorial09_Quads *pThis, Diligent::Uint32 ThreadNum);

    ThreadingTools::Signal m_RenderSubsetSignal;
    ThreadingTools::Signal m_ExecuteCommandListsSignal;
    ThreadingTools::Signal m_GotoNextFrameSignal;
    std::mutex m_NumThreadsCompletedMtx;
    std::atomic_int m_NumThreadsCompleted;
    std::atomic_int m_NumThreadsReady;
    std::vector<std::thread> m_WorkerThreads;
    std::vector< Diligent::RefCntAutoPtr<Diligent::ICommandList> > m_CmdLists;

    Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
    Diligent::RefCntAutoPtr<Diligent::IBuffer> m_InstanceConstants;
        
    static constexpr int NumTextures = 4;
    Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> m_SRB[NumTextures];
    Diligent::RefCntAutoPtr<Diligent::ITextureView> m_TextureSRV[NumTextures];
    int m_NumQuads = 1000;

    int m_MaxThreads = 8;
    int m_NumWorkerThreads = 4;

    struct InstanceData
    {
        float2 Pos;
        float2 MoveDir;
        float Size;
        float Angle;
        float RotSpeed;
        int TextureInd;
    };
    std::vector<InstanceData> m_InstanceData;
};
