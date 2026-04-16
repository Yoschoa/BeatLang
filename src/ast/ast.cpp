#include "../../includes/ast/ast.hpp"


namespace beatlang::ast {

    // Helper function to create an identation 
    std::string getIndent(int indent) {
        // return a string of length which is made up of space characters 
        return std::string(indent, ' ');
    }

    void TempoNode::print(int indent) const{
        std::cout << getIndent(indent) << "TempoNode: " << bpm << " BPM\n"; 
    }

    void TrackNode::print(int indent) const{
        std::cout << getIndent(indent) << "TrackNode: " << drumPart << " : [" << sequence << "]\n";
    }

    void PatternNode::addTrack(std::unique_ptr<TrackNode> track) {
        patternTracks.push_back(std::move(track));
    }

    void PatternNode::print(int indent) const {
        std::cout << getIndent(indent) << "PatternNode: " << patternName << "\n";
        
        // loop through the vector of trackNode and print them out 
        for(const auto &track : patternTracks){
            track->print(indent + 4);
        }
    }

    void PlayNode::print(int indent) const {
        std::cout << getIndent(indent) << "PlayNode: target-> " << targetPattern << "\n";
    }

    void LoopNode::addStatement(std::unique_ptr<StatementNode> statement) {
        body.push_back(std::move(statement));
    }

    void LoopNode::print(int indent) const {
        std::cout << getIndent(indent) << "LoopNode: count-> " << loopCount << "\n";
        
        for (const auto &stmnt : body) {
            stmnt->print(indent + 4);
        }
    }

    void SongNode::buildSong(std::unique_ptr<StatementNode> statement) {
        statements.push_back(std::move(statement));
    };

    void SongNode::print(int indent) const{
        std::cout << getIndent(indent) << "SongNode:\n";
        for(const auto &stmnt : statements) {
            stmnt->print(indent + 4);
        }
    }

    void ProgramNode::print(int indent) const {

        std::cout << "=================================\n";
        std::cout << "       ABSTRACT SYNTAX TREE      \n";
        std::cout << "=================================\n";
        std::cout << getIndent(indent) << "ProgramNode (Root)\n";
        
        if (tempo) {
            tempo->print(indent + 4);
        }
        
        for (const auto& pattern : patterns) {
            pattern->print(indent + 4);
        }
        
        if (song) {
            song->print(indent + 4);
        }
        std::cout << "=================================\n";
    }


}