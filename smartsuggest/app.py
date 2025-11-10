from flask import Flask, request, jsonify
from flask_cors import CORS
import ctypes
import re

app = Flask(__name__)
CORS(app)

# --- Load C DLL ---
lib = ctypes.CDLL('./smart_suggest.dll')
lib.getClosestWord.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.getClosestWord.restype = None

# --- Load synonyms from thesaurus_db.txt ---
synonyms_dict = {}
try:
    with open('thesaurus_db.txt', 'r', encoding='utf-8') as f:
        for line in f:
            parts = line.strip().split(':')
            if len(parts) == 2:
                word = parts[0].strip().lower()
                synonyms = [s.strip() for s in parts[1].split(',') if s.strip()]
                synonyms_dict[word] = synonyms
except FileNotFoundError:
    print("⚠️ thesaurus_db.txt not found, skipping synonyms")

# --- Clean word helper (ignores special chars, lowercase everything) ---
def clean_word(word):
    return re.sub(r'[^a-zA-Z]', '', word).lower()

# --- Suggest route ---
@app.route('/suggest', methods=['POST'])
def suggest():
    data = request.get_json()
    raw_word = data.get("word", "")
    cleaned_word = clean_word(raw_word)
    suggestions = []

    # 1️⃣ Read all saved words
    try:
        with open("wordlist.txt", "r", encoding='utf-8') as f:
            words = [w.strip().lower() for w in f.readlines() if w.strip()]
    except FileNotFoundError:
        words = []

    # 2️⃣ Find prefix matches
    prefix_matches = [w for w in words if w.startswith(cleaned_word)]
    suggestions.extend(prefix_matches)

    # 3️⃣ Include synonyms ONLY if the typed word exactly matches a saved word
    if cleaned_word in words and cleaned_word in synonyms_dict:
        suggestions.extend(synonyms_dict[cleaned_word])

    # 4️⃣ If no prefix matches → use edit distance DLL
    error_flag = False
    base_suggestion = ""
    if not suggestions and cleaned_word:
        corrected = ctypes.create_string_buffer(100)
        lib.getClosestWord(cleaned_word.encode('utf-8'), corrected)
        base_suggestion = corrected.value.decode('utf-8')
        if base_suggestion and base_suggestion != cleaned_word:
            suggestions.append(base_suggestion)
            error_flag = True

    # 5️⃣ Remove duplicates & limit to 5
    seen = set()
    unique_suggestions = []
    for s in suggestions:
        if s not in seen:
            seen.add(s)
            unique_suggestions.append(s)
    suggestions = unique_suggestions[:5]

    # 6️⃣ If nothing meaningful, return empty list
    if not suggestions:
        error_flag = False

    return jsonify({
        "suggestions": suggestions,
        "error": error_flag
    })

# --- Store route (for saving new words on Enter) ---
@app.route('/store', methods=['POST'])
def store():
    data = request.get_json()
    word = clean_word(data.get("word", ""))
    if not word:
        return jsonify({"status": "ignored"})

    with open("wordlist.txt", "a", encoding='utf-8') as f:
        f.write(f"{word}\n")

    return jsonify({"status": "saved", "word": word})

# --- Run backend ---
if __name__ == '__main__':
    app.run(debug=True)
