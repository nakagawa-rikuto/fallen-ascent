#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// Data
#include "Engine/DataInfo/PipelineStateObjectType.h"


///=====================================================/// 
/// レンダリングサービスロケータ
///=====================================================///
class Render {
public:

	/// <summary>
	/// 指定されたパイプラインタイプ、ブレンドモード、およびプリミティブトポロジーに基づいてコマンドリスト上のパイプラインステートオブジェクト（PSO）を設定します。
	/// </summary>
	/// <param name="commandList">コマンドの記録先となる ID3D12GraphicsCommandList へのポインタ。PSO の設定はこのコマンドリストに記録されます。</param>
	/// <param name="type">設定するパイプラインの種類を示す PipelineType 列挙値（例: グラフィックス、コンピュートなど）。</param>
	/// <param name="mode">使用するブレンドモードを指定する BlendMode 列挙値。</param>
	/// <param name="topology">プリミティブトポロジーを指定する D3D12_PRIMITIVE_TOPOLOGY 値。既定値は D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST。</param>
	static void SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode, D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/// <summary>
	/// 指定したキーに対応するディスクリプタテーブルを、指定したルートパラメータ位置に設定します。
	/// </summary>
	/// <param name="commandList">コマンドを記録する ID3D12GraphicsCommandList へのポインター。このコマンドリスト上でルートディスクリプタテーブルの設定が行われます。</param>
	/// <param name="RootParameterIndex">ルートシグネチャ内の、ディスクリプタテーブルを設定するルートパラメータのインデックス（UINT）。</param>
	/// <param name="key">設定するディスクリプタテーブルを識別する文字列キー。</param>
	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string key);
};

