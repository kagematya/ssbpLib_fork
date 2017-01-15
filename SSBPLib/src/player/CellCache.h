#pragma once

namespace ss{

/**
 * CellRef
 */
struct CellRef
{
	const Cell* cell;
	TextuerData texture;
	SSRect rect;
	std::string texname;
};


/**
 * CellCache
 */
class CellCache
{
public:
	CellCache()
	{
	}
	~CellCache()
	{
		releseReference();
	}

	static CellCache* create(const ProjectData* data, const std::string& imageBaseDir)
	{
		CellCache* obj = new CellCache();
		if(obj)
		{
			obj->init(data, imageBaseDir);
		}
		return obj;
	}

	CellRef* getReference(int index)
	{
		if(index < 0 || index >= (int)_refs.size())
		{
			SS_LOG("Index out of range > %d", index);
			SS_ASSERT(0);
		}
		CellRef* ref = _refs.at(index);
		return ref;
	}

	//指定した名前のセルの参照テクスチャを変更する
	bool setCellRefTexture(const ProjectData* data, const char* cellName, long texture)
	{
		bool rc = false;

		ToPointer ptr(data);
		const Cell* cells = ptr.toCells(data);

		//名前からインデックスの取得
		int cellindex = -1;
		for(int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = ptr.toCellMap(cell);
			const char* name = ptr.toString(cellMap->name);
			if(strcmp(cellName, name) == 0)
			{
				CellRef* ref = getReference(i);
				ref->texture.handle = texture;
				rc = true;
			}
		}

		return(rc);
	}

	//指定したデータのテクスチャを破棄する
	bool releseTexture(const ProjectData* data)
	{
		bool rc = false;

		ToPointer ptr(data);
		const Cell* cells = ptr.toCells(data);
		for(int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = ptr.toCellMap(cell);
			{
				CellRef* ref = _refs.at(i);
				if(ref->texture.handle != -1)
				{
					SSTextureRelese(ref->texture.handle);
					ref->texture.handle = -1;
					rc = true;
				}
			}
		}
		return(rc);
	}

protected:
	void init(const ProjectData* data, const std::string& imageBaseDir)
	{

		SS_ASSERT_LOG(data != NULL, "Invalid data");

		_textures.clear();
		_refs.clear();
		_texname.clear();

		ToPointer ptr(data);
		const Cell* cells = ptr.toCells(data);

		for(int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = ptr.toCellMap(cell);

			if(cellMap->index >= (int)_textures.size())
			{
				const char* imagePath = ptr.toString(cellMap->imagePath);
				addTexture(imagePath, imageBaseDir, (SsTexWrapMode::_enum)cellMap->wrapmode, (SsTexFilterMode::_enum)cellMap->filtermode);
			}

			//セル情報だけ入れておく
			//テクスチャの読み込みはゲーム側に任せる
			CellRef* ref = new CellRef();
			ref->cell = cell;
			ref->texture = _textures.at(cellMap->index);
			ref->texname = _texname.at(cellMap->index);
			ref->rect = SSRect(cell->x, cell->y, cell->width, cell->height);
			_refs.push_back(ref);
		}

	}
	//キャッシュの削除
	void releseReference(void)
	{
		for(int i = 0; i < _refs.size(); i++)
		{
			CellRef* ref = _refs.at(i);
			if(ref->texture.handle != -1)
			{
				SSTextureRelese(ref->texture.handle);
				ref->texture.handle = -1;
			}
			delete ref;
		}
		_refs.clear();
	}

	void addTexture(const std::string& imagePath, const std::string& imageBaseDir, SsTexWrapMode::_enum  wrapmode, SsTexFilterMode::_enum filtermode)
	{
		std::string path = "";

		if(isAbsolutePath(imagePath))
		{
			// 絶対パスのときはそのまま扱う
			path = imagePath;
		}
		else
		{
			// 相対パスのときはimageBaseDirを付与する
			path.append(imageBaseDir);
			size_t pathLen = path.length();
			if(pathLen && path.at(pathLen - 1) != '/' && path.at(pathLen - 1) != '\\')
			{
				path.append("/");
			}
			path.append(imagePath);
		}

		//テクスチャの読み込み
		long tex = SSTextureLoad(path.c_str(), wrapmode, filtermode);
		SS_LOG("load: %s", path.c_str());
		TextuerData texdata;
		texdata.handle = tex;
		int w;
		int h;
		SSGetTextureSize(texdata.handle, w, h);
		texdata.size_w = w;
		texdata.size_h = h;

		_textures.push_back(texdata);
		_texname.push_back(path);

	}

protected:
	std::vector<std::string>			_texname;
	std::vector<TextuerData>			_textures;
	std::vector<CellRef*>				_refs;
};

} //namespace ss
