import React, { useState } from 'react';
import { FileText, Upload, AlertCircle, CheckCircle } from 'lucide-react';
import * as mammoth from 'mammoth';

const WordDocComparator = () => {
  const [doc1Content, setDoc1Content] = useState('');
  const [doc2Content, setDoc2Content] = useState('');
  const [doc1Name, setDoc1Name] = useState('');
  const [doc2Name, setDoc2Name] = useState('');
  const [differences, setDifferences] = useState([]);
  const [compareMode, setCompareMode] = useState('paragraph');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  // Word 파일 읽기
  const readWordFile = async (file) => {
    try {
      const arrayBuffer = await file.arrayBuffer();
      const result = await mammoth.extractRawText({ arrayBuffer });
      return result.value;
    } catch (err) {
      throw new Error(`파일 읽기 실패: ${err.message}`);
    }
  };

  // 파일 업로드 핸들러
  const handleFileUpload = async (file, docNumber) => {
    if (!file) return;

    // 파일 확장자 확인
    const validExtensions = ['.doc', '.docx'];
    const fileName = file.name.toLowerCase();
    const isValid = validExtensions.some(ext => fileName.endsWith(ext));

    if (!isValid) {
      setError('Word 파일(.doc, .docx)만 업로드 가능합니다.');
      return;
    }

    setLoading(true);
    setError('');

    try {
      const content = await readWordFile(file);
      
      if (docNumber === 1) {
        setDoc1Content(content);
        setDoc1Name(file.name);
      } else {
        setDoc2Content(content);
        setDoc2Name(file.name);
      }
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  // 문단 단위 비교
  const compareByParagraph = (text1, text2) => {
    const paragraphs1 = text1.split(/\n+/).filter(p => p.trim());
    const paragraphs2 = text2.split(/\n+/).filter(p => p.trim());
    const diffs = [];
    const maxLength = Math.max(paragraphs1.length, paragraphs2.length);

    for (let i = 0; i < maxLength; i++) {
      const para1 = paragraphs1[i] || '';
      const para2 = paragraphs2[i] || '';

      if (para1.trim() !== para2.trim()) {
        diffs.push({
          index: i + 1,
          doc1Text: para1,
          doc2Text: para2,
          type: !para1 ? 'added' : !para2 ? 'removed' : 'modified'
        });
      }
    }

    return diffs;
  };

  // 문장 단위 비교
  const compareBySentence = (text1, text2) => {
    const sentences1 = text1.split(/[.!?]+/).filter(s => s.trim());
    const sentences2 = text2.split(/[.!?]+/).filter(s => s.trim());
    const diffs = [];
    const maxLength = Math.max(sentences1.length, sentences2.length);

    for (let i = 0; i < maxLength; i++) {
      const sent1 = sentences1[i] ? sentences1[i].trim() : '';
      const sent2 = sentences2[i] ? sentences2[i].trim() : '';

      if (sent1 !== sent2) {
        diffs.push({
          index: i + 1,
          doc1Text: sent1,
          doc2Text: sent2,
          type: !sent1 ? 'added' : !sent2 ? 'removed' : 'modified'
        });
      }
    }

    return diffs;
  };

  // 단어 단위 비교
  const compareByWord = (text1, text2) => {
    const words1 = text1.split(/\s+/).filter(w => w.trim());
    const words2 = text2.split(/\s+/).filter(w => w.trim());
    const diffs = [];

    // 더 효율적인 비교를 위해 일정 범위만 비교
    const compareRange = Math.min(500, Math.max(words1.length, words2.length));

    for (let i = 0; i < compareRange; i++) {
      const word1 = words1[i] || '';
      const word2 = words2[i] || '';

      if (word1 !== word2) {
        diffs.push({
          index: i + 1,
          doc1Text: word1,
          doc2Text: word2,
          type: !word1 ? 'added' : !word2 ? 'removed' : 'modified'
        });
      }
    }

    if (words1.length > compareRange || words2.length > compareRange) {
      diffs.push({
        index: compareRange + 1,
        doc1Text: `... (${words1.length - compareRange}개 더 있음)`,
        doc2Text: `... (${words2.length - compareRange}개 더 있음)`,
        type: 'info'
      });
    }

    return diffs;
  };

  // 비교 실행
  const handleCompare = () => {
    if (!doc1Content || !doc2Content) {
      setError('두 문서를 모두 업로드해주세요.');
      return;
    }

    setError('');
    let result;

    switch (compareMode) {
      case 'paragraph':
        result = compareByParagraph(doc1Content, doc2Content);
        break;
      case 'sentence':
        result = compareBySentence(doc1Content, doc2Content);
        break;
      case 'word':
        result = compareByWord(doc1Content, doc2Content);
        break;
      default:
        result = [];
    }

    setDifferences(result);
  };

  const getTypeColor = (type) => {
    switch (type) {
      case 'added': return 'bg-green-100 border-green-300';
      case 'removed': return 'bg-red-100 border-red-300';
      case 'modified': return 'bg-yellow-100 border-yellow-300';
      case 'info': return 'bg-blue-100 border-blue-300';
      default: return 'bg-gray-100 border-gray-300';
    }
  };

  const getTypeLabel = (type) => {
    switch (type) {
      case 'added': return '추가됨';
      case 'removed': return '삭제됨';
      case 'modified': return '수정됨';
      case 'info': return '정보';
      default: return '변경';
    }
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100 p-8">
      <div className="max-w-7xl mx-auto">
        <div className="text-center mb-8">
          <div className="flex items-center justify-center gap-3 mb-4">
            <FileText className="w-10 h-10 text-indigo-600" />
            <h1 className="text-4xl font-bold text-gray-800">Word 문서 비교 도구</h1>
          </div>
          <p className="text-gray-600">두 개의 Word 문서(.doc, .docx)를 업로드하여 차이점을 확인하세요</p>
        </div>

        {error && (
          <div className="bg-red-100 border-2 border-red-300 rounded-lg p-4 mb-6 flex items-center gap-3">
            <AlertCircle className="w-6 h-6 text-red-600" />
            <p className="text-red-800 font-medium">{error}</p>
          </div>
        )}

        <div className="bg-white rounded-xl shadow-lg p-6 mb-6">
          <div className="grid grid-cols-2 gap-6 mb-6">
            {/* 문서 1 업로드 */}
            <div>
              <label className="block text-sm font-semibold text-gray-700 mb-3">
                문서 1 업로드
              </label>
              <div className="border-2 border-dashed border-gray-300 rounded-lg p-6 text-center hover:border-indigo-400 transition">
                <Upload className="w-12 h-12 text-gray-400 mx-auto mb-3" />
                <input
                  type="file"
                  accept=".doc,.docx"
                  onChange={(e) => handleFileUpload(e.target.files[0], 1)}
                  className="hidden"
                  id="file1"
                />
                <label
                  htmlFor="file1"
                  className="cursor-pointer text-indigo-600 font-medium hover:text-indigo-700"
                >
                  파일 선택
                </label>
                {doc1Name && (
                  <div className="mt-3 flex items-center justify-center gap-2 text-green-600">
                    <CheckCircle className="w-5 h-5" />
                    <span className="text-sm font-medium">{doc1Name}</span>
                  </div>
                )}
              </div>
            </div>

            {/* 문서 2 업로드 */}
            <div>
              <label className="block text-sm font-semibold text-gray-700 mb-3">
                문서 2 업로드
              </label>
              <div className="border-2 border-dashed border-gray-300 rounded-lg p-6 text-center hover:border-indigo-400 transition">
                <Upload className="w-12 h-12 text-gray-400 mx-auto mb-3" />
                <input
                  type="file"
                  accept=".doc,.docx"
                  onChange={(e) => handleFileUpload(e.target.files[0], 2)}
                  className="hidden"
                  id="file2"
                />
                <label
                  htmlFor="file2"
                  className="cursor-pointer text-indigo-600 font-medium hover:text-indigo-700"
                >
                  파일 선택
                </label>
                {doc2Name && (
                  <div className="mt-3 flex items-center justify-center gap-2 text-green-600">
                    <CheckCircle className="w-5 h-5" />
                    <span className="text-sm font-medium">{doc2Name}</span>
                  </div>
                )}
              </div>
            </div>
          </div>

          <div className="mb-6">
            <label className="block text-sm font-semibold text-gray-700 mb-3">
              비교 방식 선택
            </label>
            <div className="flex gap-3">
              <button
                onClick={() => setCompareMode('paragraph')}
                className={`flex-1 px-4 py-3 rounded-lg font-medium transition ${
                  compareMode === 'paragraph'
                    ? 'bg-indigo-600 text-white'
                    : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                }`}
              >
                문단 단위
              </button>
              <button
                onClick={() => setCompareMode('sentence')}
                className={`flex-1 px-4 py-3 rounded-lg font-medium transition ${
                  compareMode === 'sentence'
                    ? 'bg-indigo-600 text-white'
                    : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                }`}
              >
                문장 단위
              </button>
              <button
                onClick={() => setCompareMode('word')}
                className={`flex-1 px-4 py-3 rounded-lg font-medium transition ${
                  compareMode === 'word'
                    ? 'bg-indigo-600 text-white'
                    : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                }`}
              >
                단어 단위
              </button>
            </div>
          </div>

          <button
            onClick={handleCompare}
            disabled={loading || !doc1Content || !doc2Content}
            className="w-full bg-indigo-600 text-white py-4 rounded-lg font-semibold hover:bg-indigo-700 transition shadow-md disabled:bg-gray-400 disabled:cursor-not-allowed"
          >
            {loading ? '처리 중...' : '문서 비교하기'}
          </button>
        </div>

        {differences.length > 0 && (
          <div className="bg-white rounded-xl shadow-lg p-6">
            <div className="flex items-center gap-2 mb-6">
              <AlertCircle className="w-6 h-6 text-indigo-600" />
              <h2 className="text-2xl font-bold text-gray-800">
                발견된 차이점: {differences.filter(d => d.type !== 'info').length}개
              </h2>
            </div>

            <div className="space-y-4">
              {differences.map((diff, index) => (
                <div
                  key={index}
                  className={`border-2 rounded-lg p-5 ${getTypeColor(diff.type)}`}
                >
                  <div className="flex items-center gap-2 mb-3">
                    <span className="px-3 py-1 bg-white rounded-full text-sm font-semibold">
                      {compareMode === 'paragraph' && `문단 ${diff.index}`}
                      {compareMode === 'sentence' && `문장 ${diff.index}`}
                      {compareMode === 'word' && `단어 ${diff.index}`}
                    </span>
                    <span className="px-3 py-1 bg-white rounded-full text-sm font-semibold">
                      {getTypeLabel(diff.type)}
                    </span>
                  </div>
                  
                  <div className="grid grid-cols-2 gap-4">
                    <div>
                      <div className="text-xs font-semibold text-gray-600 mb-2">
                        {doc1Name || '문서 1'}:
                      </div>
                      <div className="bg-white p-4 rounded border border-gray-300 text-sm whitespace-pre-wrap break-words max-h-60 overflow-y-auto">
                        {diff.doc1Text || '(없음)'}
                      </div>
                    </div>
                    <div>
                      <div className="text-xs font-semibold text-gray-600 mb-2">
                        {doc2Name || '문서 2'}:
                      </div>
                      <div className="bg-white p-4 rounded border border-gray-300 text-sm whitespace-pre-wrap break-words max-h-60 overflow-y-auto">
                        {diff.doc2Text || '(없음)'}
                      </div>
                    </div>
                  </div>
                </div>
              ))}
            </div>
          </div>
        )}
      </div>
    </div>
  );
};

export default WordDocComparator;
