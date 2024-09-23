from fastapi import FastAPI, File, UploadFile
import subprocess

app = FastAPI()

@app.post("/ocr/")
async def ocr(file: UploadFile = File(...)):
    file_location = f"/tmp/{file.filename}"
    
    # ファイルを一時的に保存
    with open(file_location, "wb") as buffer:
        buffer.write(await file.read())

    # C++ OCRモジュールを呼び出し
    result = subprocess.run(['/app/ocr_module/build/ocr_executable', file_location],
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # エラーチェック
    if result.returncode != 0:
        return {"error": result.stderr.decode('utf-8')}

    return {"result": result.stdout.decode('utf-8')}
